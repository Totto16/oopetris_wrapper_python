#!/usr/bin/env bash

set -e

uv run bash -c "mypy src/ tests/ --strict --ignore-missing-imports --warn-unreachable"
