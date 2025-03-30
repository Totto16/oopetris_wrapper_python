#!/usr/bin/env bash

set -e

./scripts/mypy.sh

./scripts/ruff.sh
