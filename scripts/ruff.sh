#!/usr/bin/env bash

set -e

uv sync --no-editable --group lint --group own --group test --group dev
uv run --no-editable ruff check src/ tests/ "$@"
