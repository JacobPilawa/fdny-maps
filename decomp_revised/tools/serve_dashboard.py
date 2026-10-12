#!/usr/bin/env python3
"""Serve only the FDNY progress dashboard and its tracker CSV inputs locally."""

from __future__ import annotations

import argparse
import mimetypes
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import unquote, urlsplit
import webbrowser

ROOT = Path(__file__).resolve().parents[1]
ALLOWED_FILES = {
    "dashboard/index.html": "text/html; charset=utf-8",
    "dashboard/progress-live.js": "text/javascript; charset=utf-8",
    "tracker/progress_history.csv": "text/csv; charset=utf-8",
    "tracker/function_reviews.csv": "text/csv; charset=utf-8",
    "tracker/milestones.csv": "text/csv; charset=utf-8",
    "tracker/genesis_api_crosswalk.csv": "text/csv; charset=utf-8",
}


class DashboardHandler(BaseHTTPRequestHandler):
    def do_GET(self) -> None:
        requested = unquote(urlsplit(self.path).path).lstrip("/")
        if requested in {"", "dashboard", "dashboard/"}:
            requested = "dashboard/index.html"
        content_type = ALLOWED_FILES.get(requested)
        file_path = ROOT / requested
        if content_type is None or not file_path.is_file():
            self.send_error(404, "Dashboard resource not found")
            return
        payload = file_path.read_bytes()
        self.send_response(200)
        self.send_header("Content-Type", content_type or mimetypes.guess_type(file_path.name)[0] or "application/octet-stream")
        self.send_header("Content-Length", str(len(payload)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(payload)

    def log_message(self, format_string: str, *args: object) -> None:
        return


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", type=int, default=8765, help="loopback HTTP port (default: 8765)")
    parser.add_argument("--no-open", action="store_true", help="print the local URL without opening a browser")
    args = parser.parse_args()
    server = ThreadingHTTPServer(("127.0.0.1", args.port), DashboardHandler)
    url = f"http://127.0.0.1:{server.server_port}/dashboard/"
    print(f"Live FDNY tracker: {url}", flush=True)
    print("CSV changes appear automatically; press Ctrl+C to stop this local server.", flush=True)
    if not args.no_open:
        webbrowser.open(url)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("Dashboard server stopped.", flush=True)
    finally:
        server.server_close()


if __name__ == "__main__":
    main()
