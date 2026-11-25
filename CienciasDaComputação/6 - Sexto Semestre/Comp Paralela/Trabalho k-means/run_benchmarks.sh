#!/usr/bin/env bash
# Uso: bash run_benchmarks.sh [dataset] [K] [max_iter] [--repeticoes N]
set -e
make -s all
exec python3 run_benchmarks.py "$@"
