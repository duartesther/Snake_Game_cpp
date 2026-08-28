#!/usr/bin/env bash
set -euo pipefail
docker compose build --no-cache
docker compose run --rm snake-text
