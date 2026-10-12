# Ghidra MCP bridge

This workspace includes the bethington bridge launcher, a copy of the GhidraMCP 5.8.0 archive, and a project-local copy of the installed extension under `.local/ghidra-user-home/`. The project-scoped Codex config launches `launch_bridge.sh`, which uses the ignored `.local/uv-cache/` dependency cache.

For this environment, the extension exposes a headless server entry point. `tools/ghidra-mcp/start_server.sh` loads the saved `FDNYPort` project into GhidraMCP on `127.0.0.1:8089`. The server was started and `/check_connection` returned `Connection OK`. The launcher sets the local Ghidra user home and Java headless mode; it uses the installed Java 21 runtime. Leave that process running while using the MCP bridge. The GUI path is still available if you prefer to open the project in Ghidra.

The stdio bridge handshake and TCP `connect_instance(project="FDNYPort")` have been verified; it registered 180 of 181 Ghidra server tools. Read-only queries confirmed the EXE program, function count, and entry point. Restart or reopen this trusted Codex project so its configured MCP server tools appear in the current conversation. The bridge's script-execution and write tools are configured for approval before use.

The GUI launcher was attempted in this environment with Java 21 and Java 25; macOS AWT aborted during application registration. The headless server avoids that GUI dependency. The copied extension's `Module.manifest` logs a format warning, but its headless server starts and answers requests.


## Current status, 2026-10-12

The headless GhidraMCP service answered a read-only connection check and reported FDNYFirefighter.exe at the project-pinned input path. The project lock belongs to a live Java listener on 127.0.0.1:8089. A second server launch failed to acquire that lock, so do not start another instance or remove the lock. Read-only function queries were completed against the active listener. The restricted shell cannot reach loopback directly; use the configured MCP bridge after trusting/reloading the workspace, or use the approved local query route already established for this project.
