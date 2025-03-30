#!/usr/bin/env bash

set -e

uv run "bash -c 'coverage run -m pytest tests/ && coverage xml'"
