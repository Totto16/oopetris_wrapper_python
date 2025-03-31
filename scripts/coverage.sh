#!/usr/bin/env bash

set -e

uv sync --no-editable --group own --group test
uv run --no-editable coverage run -m pytest tests/
uv run --no-editable coverage xml
