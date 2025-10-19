#!/usr/bin/env bash
set -euo pipefail
# Placeholder test runner; uses make target when tests are added
make -C "$(dirname "$0")/.." test || true
