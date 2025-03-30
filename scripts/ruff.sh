#!/usr/bin/env bash

set -e

uv run bash -c "ruff check src/ tests/ $*"
