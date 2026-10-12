# Ghidra MCP bridge

This folder contains the bridge source and launch scripts for the [bethington/ghidra-mcp](https://github.com/bethington/ghidra-mcp) project. The upstream Ghidra extension archive, installed extension, Ghidra application, local dependency cache, and analyzed project database are machine-local and are not committed here.

The saved local project is named `FDNYPort`; its database and retail inputs are intentionally excluded from this public repository. `start_server.sh` expects the project at `analysis/ghidra/FDNYPort.gpr` relative to this workspace, with Ghidra installed at `/Applications/ghidra` by default. Set `GHIDRA_HOME` or `GHIDRA_PROJECT_PATH` to override those local defaults. It starts a headless service on `127.0.0.1:8089`. Check `/check_connection` before attempting another instance; a live server owns the project lock.

The stdio bridge is launched with `launch_bridge.sh` and requires `uv` plus the local headless service. Review the upstream project instructions before installing the matching Ghidra extension. Keep Ghidra writes behind explicit approval and export only the inventories and evidence needed for the public tracker.
