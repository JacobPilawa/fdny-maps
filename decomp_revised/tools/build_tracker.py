#!/usr/bin/env python3
"""Build function/crosswalk ledgers and the static project dashboard."""

from __future__ import annotations

import csv
import html
import json
from collections import Counter, defaultdict
from datetime import datetime
from math import ceil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GENERATED = ROOT / "tracker" / "generated"
TRACKER = ROOT / "tracker"
OLD_ROOT = ROOT / "reference" / "previous-work" / "decomp"


def read_csv(path: Path) -> list[dict[str, str]]:
    with path.open(newline="", encoding="utf-8") as stream:
        return list(csv.DictReader(stream))


def write_csv(path: Path, fieldnames: list[str], rows: list[dict[str, object]]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as stream:
        writer = csv.DictWriter(
            stream, fieldnames=fieldnames, extrasaction="ignore", lineterminator="\n"
        )
        writer.writeheader()
        writer.writerows(rows)


def build_progress_svg(history: list[dict[str, str]]) -> str:
    """Render the manually snapshotted count of reviewed fresh EXE functions."""
    if not history:
        return '<p class="muted">No progress snapshots have been recorded.</p>'

    points = []
    for row in history:
        stamp = datetime.fromisoformat(row["timestamp_utc"].replace("Z", "+00:00"))
        points.append((stamp, int(row["reviewed_exe_functions"]), row.get("note", "")))
    points.sort(key=lambda point: point[0])

    width, height = 960, 300
    left, right, top, bottom = 72, 22, 24, 48
    plot_width = width - left - right
    plot_height = height - top - bottom
    max_value = max(value for _, value, _ in points)
    y_max = max(5, ceil(max_value / 5) * 5)
    span = (points[-1][0] - points[0][0]).total_seconds()

    coords = []
    for index, (stamp, value, note) in enumerate(points):
        if len(points) == 1 or span == 0:
            x = left + plot_width / 2
        else:
            x = left + (stamp - points[0][0]).total_seconds() / span * plot_width
        y = top + (y_max - value) / y_max * plot_height
        coords.append((x, y, value, note, stamp))

    grid = []
    for tick in range(5):
        value = y_max * tick / 4
        y = top + plot_height * (1 - tick / 4)
        grid.append(
            f'<line x1="{left}" y1="{y:.1f}" x2="{width-right}" y2="{y:.1f}" class="gridline"/>'
            f'<text x="{left-12}" y="{y+4:.1f}" text-anchor="end" class="axis">{value:g}</text>'
        )

    line_points = " ".join(f"{x:.1f},{y:.1f}" for x, y, *_ in coords)
    markers = []
    for x, y, value, note, stamp in coords:
        title = html.escape(f"{stamp.isoformat()} · {value} reviewed functions · {note}")
        markers.append(f'<circle cx="{x:.1f}" cy="{y:.1f}" r="5" class="point"><title>{title}</title></circle>')

    label_indices = list(range(len(coords)))
    if len(coords) > 6:
        label_indices = sorted({round(i * (len(coords) - 1) / 5) for i in range(6)})
    labels = []
    for index in label_indices:
        x, _, _, _, stamp = coords[index]
        label = stamp.strftime("%Y-%m-%d %H:%M" if span and points[0][0].date() == points[-1][0].date() else "%Y-%m-%d")
        labels.append(f'<text x="{x:.1f}" y="{height-13}" text-anchor="middle" class="axis">{label}</text>')

    return (
        f'<svg class="progress-chart" viewBox="0 0 {width} {height}" role="img" aria-label="Reviewed Ghidra-defined EXE functions over time">'
        f'<text x="{left}" y="15" class="axis-title">Reviewed EXE functions</text>'
        f'{"".join(grid)}'
        f'<line x1="{left}" y1="{top}" x2="{left}" y2="{top+plot_height}" class="axisline"/>'
        f'<line x1="{left}" y1="{top+plot_height}" x2="{width-right}" y2="{top+plot_height}" class="axisline"/>'
        f'<polyline points="{line_points}" class="progress-line"/>'
        f'{"".join(markers)}{"".join(labels)}</svg>'
    )


def load_manifest() -> dict[str, str]:
    return {
        Path(row["path"]).name.lower(): row["sha256"]
        for row in read_csv(ROOT / "inputs" / "original" / "manifest.csv")
        if row["path"].startswith("inputs/original/binaries/")
    }


def load_legacy_index() -> dict[str, dict[str, object]]:
    index_path = OLD_ROOT / "evidence" / "function-index.json"
    if not index_path.exists():
        return {}
    payload = json.loads(index_path.read_text(encoding="utf-8"))
    merged: dict[str, dict[str, object]] = {}
    for item in payload.get("functions", []):
        address = item["address"].upper()
        record = merged.setdefault(address, {"systems": set(), "exports": set()})
        record["systems"].update(item.get("systems", []))
        for variant in item.get("variants", []):
            record["exports"].update(variant.get("exports", []))
    return merged


def build() -> dict[str, object]:
    manifest = load_manifest()
    legacy = load_legacy_index()
    function_reviews = read_csv(TRACKER / "function_reviews.csv")
    reviewed_addresses = [row["entry_address"].upper() for row in function_reviews]
    if len(reviewed_addresses) != len(set(reviewed_addresses)):
        raise ValueError("function_reviews.csv contains duplicate function addresses")
    reviewed_address_set = set(reviewed_addresses)
    binary_metadata = {
        row["binary"].lower(): row
        for row in read_csv(TRACKER / "binaries.csv")
    } if (TRACKER / "binaries.csv").exists() else {}
    function_rows: list[dict[str, object]] = []
    per_binary: dict[str, list[dict[str, object]]] = defaultdict(list)

    for path in sorted(GENERATED.glob("*.functions.csv")):
        for row in read_csv(path):
            binary = row["binary"]
            address = row["entry_address"].upper()
            legacy_row = legacy.get(address, {}) if binary.lower().endswith(".exe") else {}
            exports = sorted(legacy_row.get("exports", set()))
            local_exports = [
                (OLD_ROOT / export).relative_to(ROOT).as_posix()
                for export in exports
                if (OLD_ROOT / export).is_file()
            ]
            is_external = row["is_external"].lower() == "true"
            state = "external" if is_external else "inventory_only"
            if binary.lower() == "fmod.dll" and not is_external:
                state = "packed_unresolved"
            elif binary.lower() == "fdnyfirefighter.exe" and address in reviewed_address_set and not is_external:
                state = "triaged"
            tracker_row: dict[str, object] = {
                "binary": binary,
                "sha256": manifest.get(binary.lower(), ""),
                "entry_address": address,
                "body_bytes": row["body_bytes"],
                "name": row["name"],
                "namespace": row["namespace"],
                "is_external": row["is_external"],
                "is_thunk": row["is_thunk"],
                "calling_convention": row["calling_convention"],
                "signature": row["signature"],
                "state": state,
                "legacy_export_count": len(local_exports),
                "legacy_systems": ";".join(sorted(legacy_row.get("systems", set()))),
                "legacy_export_paths": ";".join(local_exports),
            }
            function_rows.append(tracker_row)
            per_binary[binary].append(tracker_row)

    fields = [
        "binary", "sha256", "entry_address", "body_bytes", "name", "namespace",
        "is_external", "is_thunk", "calling_convention", "signature", "state",
        "legacy_export_count", "legacy_systems", "legacy_export_paths",
    ]
    function_rows.sort(key=lambda row: (str(row["binary"]).lower(), str(row["entry_address"])))
    write_csv(TRACKER / "functions.csv", fields, function_rows)

    current_exe_addresses = {
        str(row["entry_address"]).upper()
        for row in function_rows
        if str(row["binary"]).lower().endswith(".exe") and row["is_external"] == "false"
    }
    crosswalk_rows: list[dict[str, object]] = []
    for address, record in sorted(legacy.items()):
        exports = sorted(record["exports"])
        local_exports = [
            (OLD_ROOT / export).relative_to(ROOT).as_posix()
            for export in exports
            if (OLD_ROOT / export).is_file()
        ]
        crosswalk_rows.append({
            "entry_address": address,
            "exact_fresh_entry": address in current_exe_addresses,
            "legacy_export_files": len(exports),
            "copied_export_files": len(local_exports),
            "legacy_systems": ";".join(sorted(record["systems"])),
            "local_export_paths": ";".join(local_exports),
        })
    write_csv(TRACKER / "legacy_crosswalk.csv", [
        "entry_address", "exact_fresh_entry", "legacy_export_files",
        "copied_export_files", "legacy_systems", "local_export_paths",
    ], crosswalk_rows)

    current_exe_rows = {
        row["entry_address"].upper(): row
        for row in function_rows
        if row["binary"].lower() == "fdnyfirefighter.exe" and row["is_external"] == "false"
    }
    for review in function_reviews:
        address = review["entry_address"].upper()
        if review["binary"].lower() != "fdnyfirefighter.exe" or address not in current_exe_rows:
            raise ValueError(f"review is not a fresh EXE function boundary: {address}")
        if review["sha256"] != current_exe_rows[address]["sha256"]:
            raise ValueError(f"review hash does not match current EXE snapshot: {address}")
        evidence_paths = [
            path.strip()
            for path in review["evidence_path"].split(";")
            if path.strip()
        ]
        if not evidence_paths:
            raise ValueError(f"review has no evidence path: {address}")
        for evidence_path in evidence_paths:
            if not (ROOT / evidence_path).is_file():
                raise ValueError(f"review evidence file is missing: {evidence_path}")

    progress_history = read_csv(TRACKER / "progress_history.csv")
    parsed_history = [
        (datetime.fromisoformat(row["timestamp_utc"].replace("Z", "+00:00")), int(row["reviewed_exe_functions"]))
        for row in progress_history
    ]
    if any(current[0] <= previous[0] for previous, current in zip(parsed_history, parsed_history[1:])):
        raise ValueError("progress_history.csv timestamps must be strictly increasing")
    if any(current[1] < previous[1] for previous, current in zip(parsed_history, parsed_history[1:])):
        raise ValueError("progress_history.csv reviewed function count must be cumulative")
    if parsed_history and parsed_history[-1][1] != len(function_reviews):
        raise ValueError("latest progress snapshot must match function_reviews.csv")

    binary_summary = []
    for binary, rows in sorted(per_binary.items(), key=lambda pair: pair[0].lower()):
        internal = [row for row in rows if row["is_external"] == "false"]
        code_candidates = [row for row in internal if row["state"] != "packed_unresolved"]
        metadata = binary_metadata.get(binary.lower(), {})
        binary_summary.append({
            "binary": binary,
            "sha256": manifest.get(binary.lower(), ""),
            "role": metadata.get("role", ""),
            "classification_note": metadata.get("classification_note", ""),
            "packed": metadata.get("packed", "no"),
            "imports": metadata.get("imports", ""),
            "exports": metadata.get("exports", ""),
            "imported_libraries": metadata.get("imported_libraries", ""),
            "image_base": metadata.get("image_base", ""),
            "entry_va": metadata.get("entry_va", ""),
            "function_records": len(rows),
            "defined_functions": len(code_candidates),
            "body_function_candidates": len(internal),
            "packed_function_candidates": len(internal) - len(code_candidates),
            "external_functions": len(rows) - len(internal),
            "thunks": sum(row["is_thunk"] == "true" for row in internal),
            "legacy_exported_functions": sum(int(row["legacy_export_count"]) > 0 for row in internal),
            "functions": internal,
        })

    old_inventory_path = OLD_ROOT / "evidence" / "original-function-inventory.json"
    old_count = None
    if old_inventory_path.exists():
        old_count = json.loads(old_inventory_path.read_text(encoding="utf-8")).get("functionCount")
    old_reconciliation_path = OLD_ROOT / "evidence" / "function-inventory-reconciliation.json"
    old_reconciliation = {}
    if old_reconciliation_path.exists():
        old_reconciliation = json.loads(old_reconciliation_path.read_text(encoding="utf-8")).get("summary", {})
    fresh_exe = next((row for row in binary_summary if row["binary"].lower().endswith(".exe")), None)
    exact_count = sum(row["exact_fresh_entry"] for row in crosswalk_rows)
    summary: dict[str, object] = {
        "snapshot_date": "2026-10-12",
        "ghidra_version": "12.0.4",
        "input_manifest": "inputs/original/manifest.csv",
        "binaries": binary_summary,
        "totals": {
            "binary_count": len(binary_summary),
            "function_records": len(function_rows),
            "defined_functions": sum(row["is_external"] == "false" and row["state"] != "packed_unresolved" for row in function_rows),
            "body_function_candidates": sum(row["is_external"] == "false" for row in function_rows),
            "packed_function_candidates": sum(row["is_external"] == "false" and row["state"] == "packed_unresolved" for row in function_rows),
            "external_functions": sum(row["is_external"] == "true" for row in function_rows),
            "old_exe_defined_functions": old_count,
            "fresh_exe_defined_functions": fresh_exe["defined_functions"] if fresh_exe else None,
            "old_unique_export_addresses": len(legacy),
            "old_export_addresses_matching_fresh_exe": exact_count,
            "old_export_addresses_without_exact_fresh_entry": len(legacy) - exact_count,
            "stale_report_export_addresses": old_reconciliation.get("preservedExportAddresses"),
            "stale_report_matched_addresses": old_reconciliation.get("preservedExportAddressesWithExactGhidraEntry"),
            "copied_legacy_export_files": sum(int(row["copied_export_files"]) for row in crosswalk_rows),
            "reviewed_exe_functions": len(function_reviews),
        },
        "scope_note": "Function totals are Ghidra-defined boundaries from this binary snapshot. They are not a completeness claim or source-recovery percentage.",
    }
    build_dashboard(summary)
    public_summary = dict(summary)
    public_summary["binaries"] = [
        {key: value for key, value in binary.items() if key != "functions"}
        for binary in binary_summary
    ]
    (TRACKER / "summary.json").write_text(json.dumps(public_summary, indent=2) + "\n", encoding="utf-8")
    return summary


def build_dashboard(summary: dict[str, object]) -> None:
    bins = summary["binaries"]
    totals = summary["totals"]
    milestones = read_csv(TRACKER / "milestones.csv")
    progress_history = read_csv(TRACKER / "progress_history.csv")

    def esc(value: object) -> str:
        return html.escape(str(value))

    binary_cards = []
    for binary in bins:
        tiles = []
        for row in binary["functions"]:
            address = esc(row["entry_address"])
            name = esc(row["name"])
            binary_name = esc(binary["binary"])
            state = esc(row["state"])
            exports = int(row["legacy_export_count"])
            mark = " has-export" if exports else ""
            reviewed_mark = " reviewed" if state == "triaged" else ""
            tooltip = f"{address} · {name} · {state}" + (f" · {exports} prior export(s)" if exports else "")
            packed_mark = " packed" if state == "packed_unresolved" else ""
            tiles.append(f'<span class="tile{mark}{reviewed_mark}{packed_mark}" data-binary="{binary_name}" data-address="{address}" title="{esc(tooltip)}"></span>')
        packed_note = " · packed; Ghidra count is not a useful code inventory" if binary["packed"] == "yes" else ""
        binary_cards.append(f"""
          <article class="binary-card">
            <div class="binary-head"><div><h3>{esc(binary['binary'])}</h3><p>{esc(binary['role'])} · {esc(binary['classification_note'])}</p><code>{esc(binary['sha256'])}</code></div>
              <div class="count">{binary['defined_functions']:,}<small>analyzable body functions</small></div></div>
            <div class="atlas" aria-label="{esc(binary['binary'])} function address map">{''.join(tiles)}</div>
            <p class="binary-foot">Entry {esc(binary['entry_va'])} · {binary['imports']} imports from {binary['imported_libraries']} libraries · {binary['exports']} exports · {binary['external_functions']:,} external stubs · {binary['thunks']:,} thunks · {binary['legacy_exported_functions']:,} entries with indexed prior exports{esc(packed_note)}</p>
          </article>
        """)

    state_counts = Counter(row["state"] for binary in bins for row in binary["functions"])
    milestone_cards = []
    for milestone in milestones:
        milestone_cards.append(f"""
          <article class="milestone {esc(milestone['state'])}" data-milestone="{esc(milestone['id'])}">
            <div class="milestone-top"><span>{esc(milestone['id'])}</span><b>{esc(milestone['state']).replace('_',' ')}</b></div>
            <h3>{esc(milestone['name'])}</h3><p>{esc(milestone['exit_evidence'])}</p>
            <small>Next: {esc(milestone['next_action'])}</small>
          </article>
        """)

    html_doc = f"""<!doctype html>
<html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>FDNY Decomp and Port Tracker</title>
<style>
:root {{ color-scheme: dark; --bg:#101418; --panel:#1a2229; --line:#33414c; --text:#edf2f5; --muted:#a8b5bf; --accent:#55c2a0; --blue:#69aef5; --amber:#f3c56a; }}
* {{ box-sizing:border-box }} body {{ margin:0; background:var(--bg); color:var(--text); font:15px/1.5 ui-sans-serif,system-ui,sans-serif }}
main {{ max-width:1240px; margin:0 auto; padding:34px 22px 60px }} h1 {{ margin:0; font-size:clamp(2rem,4vw,3.4rem); letter-spacing:-.04em }}
h2 {{ margin:36px 0 12px; font-size:1.35rem }} h3 {{ margin:0 0 6px; font-size:1rem }} p {{ color:var(--muted); margin:6px 0 }} .lede {{ max-width:780px; color:var(--muted); margin:10px 0 22px }}
.stamp {{ color:var(--accent); font-weight:700; letter-spacing:.08em; text-transform:uppercase; font-size:.76rem }}
.stats {{ display:grid; grid-template-columns:repeat(auto-fit,minmax(180px,1fr)); gap:12px }} .stat,.binary-card,.milestone {{ background:var(--panel); border:1px solid var(--line); border-radius:14px; padding:17px }}
.stat strong {{ display:block; font-size:1.7rem }} .stat span {{ color:var(--muted); font-size:.85rem }} .grid {{ display:grid; grid-template-columns:repeat(auto-fit,minmax(min(100%,510px),1fr)); gap:14px }}
.binary-head {{ display:flex; justify-content:space-between; gap:16px; align-items:flex-start }} code {{ color:var(--muted); font-size:.68rem; overflow-wrap:anywhere }} .count {{ font-size:1.5rem; font-weight:700; text-align:right; white-space:nowrap }} .count small {{ display:block; font-size:.68rem; color:var(--muted); font-weight:500 }}
.atlas {{ display:grid; grid-template-columns:repeat(80,minmax(2px,1fr)); gap:2px; margin:16px 0 8px; max-height:160px; overflow:auto; padding:2px }} .tile {{ height:7px; border-radius:2px; background:#77828a }} .tile.has-export {{ background:var(--blue) }} .tile.reviewed:not(.has-export):not(.packed) {{ background:var(--accent) }} .tile.packed {{ background:#ef8d87 }} .binary-foot {{ font-size:.8rem }}
.progress-panel {{ background:var(--panel); border:1px solid var(--line); border-radius:14px; padding:14px 18px }} .progress-chart {{ width:100%; height:auto; display:block }} .progress-chart .gridline {{ stroke:#33414c; stroke-width:1 }} .progress-chart .axisline {{ stroke:#80909c; stroke-width:1 }} .progress-chart .axis {{ fill:var(--muted); font:12px ui-sans-serif,system-ui,sans-serif }} .progress-chart .axis-title {{ fill:var(--text); font:12px ui-sans-serif,system-ui,sans-serif }} .progress-line {{ fill:none;stroke:var(--accent);stroke-width:3;stroke-linejoin:round;stroke-linecap:round }} .progress-chart .point {{ fill:var(--accent);stroke:var(--bg);stroke-width:2 }}
.legend {{ display:flex; gap:18px; color:var(--muted); font-size:.8rem; margin:10px 0 0; flex-wrap:wrap }} .swatch {{ display:inline-block; width:9px;height:9px;border-radius:2px;margin-right:5px;background:#77828a }} .swatch.blue {{ background:var(--blue) }} .swatch.green {{ background:var(--accent) }}
.milestones {{ display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:12px }} .milestone-top {{ display:flex;justify-content:space-between;color:var(--accent);font-size:.76rem;text-transform:uppercase }} .milestone-top b {{ color:var(--amber);font-weight:600 }} .milestone.complete .milestone-top b {{ color:var(--accent) }} .milestone p {{ font-size:.86rem }} .milestone small {{ display:block;color:var(--muted);margin-top:12px }}
.note {{ margin-top:24px;padding:15px 18px;border-left:3px solid var(--accent);background:#17211f;color:var(--muted);border-radius:0 9px 9px 0 }} footer {{ margin-top:30px;border-top:1px solid var(--line);padding-top:14px;color:var(--muted);font-size:.8rem }}
</style></head><body><main>
<div class="stamp">FDNY Firefighter: American Hero · Behavior-preserving desktop port</div>
<h1>Reverse-engineering tracker</h1>
<p class="lede">Function atlas and project milestones from the hash-pinned game files and fresh Ghidra 12.0.4 analysis. Blue tiles mark entries with an indexed prior Ghidra export; they do not mean the function is understood or ported.</p>
<section class="stats">
<div class="stat"><strong>{totals['binary_count']}</strong><span>code-bearing modules inventoried</span></div>
<div class="stat"><strong>{totals['defined_functions']:,}</strong><span>body functions, excluding the packed FMOD candidate</span></div>
<div class="stat"><strong>{totals['old_exe_defined_functions']:,} → {totals['fresh_exe_defined_functions']:,}</strong><span>old to fresh EXE boundaries</span></div>
<div class="stat"><strong>{totals['old_export_addresses_matching_fresh_exe']:,} / {totals['old_unique_export_addresses']:,}</strong><span>prior unique export addresses found at fresh EXE entries</span></div>
<div class="stat"><strong>{totals['stale_report_matched_addresses']:,} / {totals['stale_report_export_addresses']:,}</strong><span>saved prior reconciliation report (stale)</span></div>
<div class="stat"><strong id="reviewed-count">{totals['reviewed_exe_functions']:,}</strong><span>fresh EXE functions reviewed with saved evidence</span></div>
</section>
<h2>Reverse-engineering progress over time</h2>
<article class="progress-panel"><div id="progress-chart">{build_progress_svg(progress_history)}</div><p class="binary-foot">A reviewed function has a fresh Ghidra pseudocode export and an address-specific behavior note. This is an activity count, not a source-recovery or port-parity percentage; manually bounded code slices outside the current function inventory are excluded.</p><p id="live-status" class="binary-foot" role="status">Live CSV status loading…</p></article>
<h2>Function address atlas</h2><div class="grid">{''.join(binary_cards)}</div>
<div class="legend"><span><i class="swatch"></i>Ghidra boundary, inventory only</span><span><i class="swatch green"></i>fresh pseudocode reviewed and triaged</span><span><i class="swatch blue"></i>prior indexed export also exists</span><span><i class="swatch" style="background:#ef8d87"></i>packed/unresolved</span></div>
<h2>Milestones</h2><div class="milestones" id="milestone-list">{''.join(milestone_cards)}</div>
<div class="note">{esc(summary['scope_note'])} Function state totals: {', '.join(f'{esc(key)} {value:,}' for key, value in sorted(state_counts.items()))}. Old Ghidra export joins use entry address only; the old index does not bind every export to a verified EXE hash. Exports are pseudocode artifacts, not original source.</div>
<footer>Inventory snapshot: {esc(summary['snapshot_date'])} · Live progress: <code>tracker/progress_history.csv</code> and <code>tracker/function_reviews.csv</code> · Inputs: <code>inputs/original/manifest.csv</code> · <a href="../tracker/genesis_api_crosswalk.csv">Genesis API crosswalk</a> · Full inventory refresh: <code>tools/refresh_inventory.sh</code></footer>
</main><script src="progress-live.js" defer></script></body></html>"""
    (ROOT / "dashboard" / "index.html").write_text(html_doc, encoding="utf-8")


if __name__ == "__main__":
    result = build()
    print(json.dumps(result["totals"], indent=2))
