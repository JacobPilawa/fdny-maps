(() => {
  "use strict";

  const root = "../tracker/";
  const refreshDelayMs = 60_000;
  const statusNode = document.getElementById("live-status");
  const chartNode = document.getElementById("progress-chart");
  const countNode = document.getElementById("reviewed-count");
  const svgNamespace = "http://www.w3.org/2000/svg";

  function parseCsv(source) {
    const rows = [];
    let row = [];
    let field = "";
    let quoted = false;
    for (let i = 0; i < source.length; i += 1) {
      const char = source[i];
      if (quoted) {
        if (char === '"' && source[i + 1] === '"') {
          field += '"';
          i += 1;
        } else if (char === '"') {
          quoted = false;
        } else {
          field += char;
        }
      } else if (char === '"' && field.length === 0) {
        quoted = true;
      } else if (char === ",") {
        row.push(field);
        field = "";
      } else if (char === "\n") {
        row.push(field);
        rows.push(row);
        row = [];
        field = "";
      } else if (char !== "\r") {
        field += char;
      }
    }
    if (field.length > 0 || row.length > 0) {
      row.push(field);
      rows.push(row);
    }
    if (rows.length === 0) return [];
    const headers = rows.shift().map((value, index) => (index === 0 ? value.replace(/^\uFEFF/, "") : value).trim());
    return rows
      .filter((values) => values.some((value) => value.length > 0))
      .map((values) => Object.fromEntries(headers.map((header, index) => [header, values[index] ?? ""])));
  }

  async function readCsv(fileName) {
    const response = await fetch(`${root}${fileName}`, { cache: "no-store" });
    if (!response.ok) throw new Error(`${fileName} returned HTTP ${response.status}`);
    return parseCsv(await response.text());
  }

  function makeSvgElement(name, attributes = {}, label = null) {
    const node = document.createElementNS(svgNamespace, name);
    for (const [key, value] of Object.entries(attributes)) node.setAttribute(key, String(value));
    if (label !== null) node.textContent = label;
    return node;
  }

  function renderProgressChart(rows) {
    const records = rows
      .map((row) => ({
        time: Date.parse(row.timestamp_utc),
        count: Number(row.reviewed_exe_functions),
        note: row.note || "",
        stamp: row.timestamp_utc || "",
      }))
      .filter((row) => Number.isFinite(row.time) && Number.isFinite(row.count))
      .sort((a, b) => a.time - b.time);

    if (records.length === 0) {
      chartNode.textContent = "No progress checkpoints are recorded yet.";
      return null;
    }

    const width = 960;
    const height = 300;
    const left = 72;
    const right = 22;
    const top = 24;
    const bottom = 48;
    const plotWidth = width - left - right;
    const plotHeight = height - top - bottom;
    const maxCount = Math.max(...records.map((record) => record.count));
    const yMax = Math.max(5, Math.ceil(maxCount / 5) * 5);
    const minTime = records[0].time;
    const maxTime = records[records.length - 1].time;
    const span = maxTime - minTime;
    const points = records.map((record, index) => ({
      ...record,
      x: span === 0 ? left + plotWidth / 2 : left + ((record.time - minTime) / span) * plotWidth,
      y: top + ((yMax - record.count) / yMax) * plotHeight,
      index,
    }));

    const svg = makeSvgElement("svg", {
      class: "progress-chart",
      viewBox: `0 0 ${width} ${height}`,
      role: "img",
      "aria-label": "Reviewed fresh EXE functions over time",
    });
    svg.append(makeSvgElement("text", { x: left, y: 15, class: "axis-title" }, "Reviewed EXE functions"));

    for (let tick = 0; tick <= 4; tick += 1) {
      const value = (yMax * tick) / 4;
      const y = top + plotHeight * (1 - tick / 4);
      svg.append(makeSvgElement("line", { x1: left, y1: y, x2: width - right, y2: y, class: "gridline" }));
      svg.append(makeSvgElement("text", { x: left - 12, y: y + 4, "text-anchor": "end", class: "axis" }, `${Number(value.toFixed(2))}`));
    }
    svg.append(makeSvgElement("line", { x1: left, y1: top, x2: left, y2: top + plotHeight, class: "axisline" }));
    svg.append(makeSvgElement("line", { x1: left, y1: top + plotHeight, x2: width - right, y2: top + plotHeight, class: "axisline" }));

    if (points.length > 1) {
      const pointList = points.map((point) => `${point.x.toFixed(1)},${point.y.toFixed(1)}`).join(" ");
      svg.append(makeSvgElement("polyline", { points: pointList, class: "progress-line" }));
    }
    for (const point of points) {
      const circle = makeSvgElement("circle", { cx: point.x, cy: point.y, r: 5, class: "point" });
      circle.append(makeSvgElement("title", {}, `${point.stamp} · ${point.count} reviewed functions · ${point.note}`));
      svg.append(circle);
    }

    const labelCount = Math.min(6, points.length);
    const labels = new Set(Array.from({ length: labelCount }, (_, index) =>
      labelCount === 1 ? 0 : Math.round((index * (points.length - 1)) / (labelCount - 1))));
    const formatter = new Intl.DateTimeFormat(undefined, {
      month: "2-digit", day: "2-digit", hour: "2-digit", minute: "2-digit", timeZone: undefined,
    });
    for (const index of labels) {
      const point = points[index];
      svg.append(makeSvgElement("text", {
        x: point.x,
        y: height - 13,
        "text-anchor": "middle",
        class: "axis",
      }, formatter.format(new Date(point.time))));
    }

    chartNode.replaceChildren(svg);
    return records[records.length - 1];
  }

  function updateAtlas(reviews) {
    const reviewed = new Set(reviews.map((row) => `${row.binary.toLowerCase()}|${row.entry_address.toUpperCase()}`));
    for (const tile of document.querySelectorAll(".tile[data-binary][data-address]")) {
      const key = `${tile.dataset.binary.toLowerCase()}|${tile.dataset.address.toUpperCase()}`;
      tile.classList.toggle("reviewed", reviewed.has(key));
    }
    return reviewed.size;
  }

  function updateMilestones(rows) {
    const milestones = new Map(rows.map((row) => [row.id, row]));
    for (const card of document.querySelectorAll(".milestone[data-milestone]")) {
      const row = milestones.get(card.dataset.milestone);
      if (!row) continue;
      const state = (row.state || "planned").toLowerCase();
      card.className = `milestone ${state}`;
      const stateNode = card.querySelector(".milestone-top b");
      const nameNode = card.querySelector("h3");
      const evidenceNode = card.querySelector("p");
      const nextNode = card.querySelector("small");
      if (stateNode) stateNode.textContent = state.replaceAll("_", " ");
      if (nameNode) nameNode.textContent = row.name || "";
      if (evidenceNode) evidenceNode.textContent = row.exit_evidence || "";
      if (nextNode) nextNode.textContent = `Next: ${row.next_action || ""}`;
    }
  }

  async function refresh() {
    if (window.location.protocol === "file:") {
      statusNode.textContent = "CSV auto-update needs the local server. Run python3 tools/serve_dashboard.py, then open the local address it prints.";
      window.setTimeout(refresh, refreshDelayMs);
      return;
    }
    try {
      const [history, reviews, milestones] = await Promise.all([
        readCsv("progress_history.csv"),
        readCsv("function_reviews.csv"),
        readCsv("milestones.csv"),
      ]);
      const reviewedCount = updateAtlas(reviews);
      countNode.textContent = reviewedCount.toLocaleString();
      updateMilestones(milestones);
      const latest = renderProgressChart(history);
      const checkedAt = new Date().toLocaleTimeString();
      if (latest) {
        const historyCount = Number(latest.count);
        const mismatch = historyCount !== reviewedCount
          ? ` · checkpoint ${historyCount.toLocaleString()} vs ledger ${reviewedCount.toLocaleString()}`
          : "";
        statusNode.textContent = `Live from CSV · latest checkpoint ${latest.stamp} · checked ${checkedAt}${mismatch}`;
      } else {
        statusNode.textContent = `Live from CSV · ${reviewedCount.toLocaleString()} reviewed · checked ${checkedAt}`;
      }
    } catch (error) {
      statusNode.textContent = `CSV refresh failed: ${error.message}. Keep this page on the local server and retry.`;
    } finally {
      window.setTimeout(refresh, refreshDelayMs);
    }
  }

  refresh();
})();
