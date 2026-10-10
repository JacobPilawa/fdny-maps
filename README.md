# FDNY maps

Interactive 2D/3D maps with entity labels, triggers, collision overlays, and original PDFs.

**Live:** https://fdny-maps.piececounter.workers.dev/

**Local:** run `./map_explorer/start.sh`, then open http://127.0.0.1:4180.

**Hosting:** Cloudflare Workers Static Assets, using the free tier with no backend or database. The build checks asset limits and adds versioned URLs with long-lived caching. Deployment is manual, not automatic from GitHub.

With Python 3 and Node.js installed:

```sh
cd map_explorer
python3 deploy/build_static.py
npx wrangler login
npx wrangler deploy --config wrangler.json
```

For your own Cloudflare account, change `account_id` and `name` in `map_explorer/wrangler.json` first.
