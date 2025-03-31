#!/usr/bin/env bash

set -e

uv sync --no-editable --group own --group test
uv run --no-editable pytest tests/
