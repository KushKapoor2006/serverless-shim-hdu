#!/usr/bin/env bash
set -euo pipefail
mkdir -p ../logs
echo "Parsing latest RTL CSV and computing metrics..."
python3 parse_rtl_logs.py
echo "Comparing with Python sim outputs (if any)..."
python3 compare_with_sim.py
echo "All done. Check sw/logs/ for results."
