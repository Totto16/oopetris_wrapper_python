#!/usr/bin/env bash

set -e

uv sync --no-editable --group lint --group own --group test
uv run --no-editable ruff check src/ tests/ "$@"
