# FDNY maps

Interactive 2D/3D maps with entity labels, triggers, collision overlays, and original PDFs.

**Live:** https://fdny-maps.piececounter.workers.dev/

**Local:** run `./map_explorer/start.sh`, then open http://127.0.0.1:4180.

## How hosting works

The live site's files and map data are **stored on Cloudflare**. GitHub holds the source and a copy of the data; visitors do not load files from GitHub.

- `deploy/build_static.py` copies the website, map JSON, textures, PDFs, and downloadable annotations/scripts into `map_explorer/dist/`. Original BSPs and research stay in the repository for rebuilding.
- `wrangler deploy` uploads that folder to **Cloudflare Workers Static Assets**. Cloudflare stores and serves the uploaded files at the live URL. There is no custom Worker backend, database, or R2 bucket.
- The viewer runs in the visitor's browser. It requests map data and optional textures/collision overlays from the same Cloudflare site as needed.
- Assets get content-versioned URLs and one-year browser caching. The entry page revalidates, so a new deployment loads the new version.

This replaces the separate R2-upload approach: the frontend and its static data are uploaded together as site assets. R2 is a separate object-storage service; this tool does not need it.

Static asset requests and storage are free under [Cloudflare's static-assets pricing](https://developers.cloudflare.com/workers/static-assets/billing-and-limitations/). The build checks the Free plan's limits of 20,000 files and 25 MiB per file.

## Deploy

Deployment is manual: **pushing to GitHub does not update the live site**. Build and upload your local checkout with the commands below.

With Python 3 and Node.js installed:

```sh
cd map_explorer
python3 deploy/build_static.py
npx wrangler login
npx wrangler deploy --config wrangler.json
```

For your own Cloudflare account, change `account_id` and `name` in `map_explorer/wrangler.json` first.
