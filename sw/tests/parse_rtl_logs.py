#!/usr/bin/env python3
# sw/tests/parse_rtl_logs.py
"""
Improved RTL CSV parser with queue-based pairing:
- For each send_packet: push to queue
- For each dispatch_slot_* or auth_fallback: pop earliest unmatched send and pair
- Remaining sends -> timeout

Produces:
 - sw/logs/per_request_latencies.csv
 - sw/logs/rtl_metrics_summary.json
"""
import csv
import glob
import json
import math
import os
import statistics
from pathlib import Path

# convert cycles -> microseconds
CYCLE_TO_US = 5e-3  # 5 ns = 0.005 us

REPO_ROOT = Path(__file__).resolve().parents[2]
RTL_LOG_DIR = REPO_ROOT / "evaluation" / "logs"
OUT_DIR = REPO_ROOT / "sw" / "logs"
OUT_DIR.mkdir(parents=True, exist_ok=True)

def find_latest_rtl_csv():
    files = sorted(RTL_LOG_DIR.glob("rtl_run_*.csv"), key=os.path.getmtime)
    if not files:
        raise FileNotFoundError(f"No RTL CSV logs found in {RTL_LOG_DIR}")
    return files[-1]

def load_events(csv_path):
    events = []
    with open(csv_path, newline='') as f:
        r = csv.reader(f)
        header = next(r)  # event,cycle,ns
        for row in r:
            if len(row) < 2:
                continue
            event = row[0].strip()
            cycle = int(float(row[1]))
            events.append((event, cycle))
    return events

def pair_queue_style(events):
    """
    Queue-based pairing:
      - maintain queue of send_packet indices (with cycle)
      - on dispatch_slot_X or auth_fallback, pop earliest send and pair
    Returns list of per-request dicts.
    """
    per_requests = []
    send_queue = []  # list of dicts {send_cycle, idx}
    idx_counter = 0

    # we will produce an entry for each send_packet
    for (ev, cyc) in events:
        if ev == "send_packet":
            entry = {
                "send_cycle": cyc,
                "resp_type": None,
                "resp_cycle": None,
                "latency_cycles": None,
                "latency_us": None,
                "dispatch_slot": None,
                "idx": idx_counter
            }
            idx_counter += 1
            send_queue.append(entry)
            per_requests.append(entry)

        elif ev.startswith("dispatch_slot") or ev == "auth_fallback":
            # consume earliest unmatched send if any
            if send_queue:
                entry = send_queue.pop(0)
                entry["resp_cycle"] = cyc

                if ev == "auth_fallback":
                    entry["resp_type"] = "fallback"
                else:
                    entry["resp_type"] = "dispatch"
                    # parse slot number if present
                    try:
                        # dispatch_slot_N
                        slot_str = ev.split("_")[-1]
                        entry["dispatch_slot"] = int(slot_str)
                    except Exception:
                        entry["dispatch_slot"] = None

                entry["latency_cycles"] = entry["resp_cycle"] - entry["send_cycle"]
                entry["latency_us"] = entry["latency_cycles"] * CYCLE_TO_US
            else:
                # no pending send: stray response ignored
                pass

        else:
            # other events ignored for pairing
            pass

    # Any remaining items in send_queue are timeouts
    for entry in send_queue:
        entry["resp_type"] = "timeout"
        entry["resp_cycle"] = None
        entry["latency_cycles"] = None
        entry["latency_us"] = None

    # Sort per_requests by send_cycle for deterministic output
    per_requests_sorted = sorted(per_requests, key=lambda r: r["send_cycle"])
    return per_requests_sorted

def summarize(per_requests):
    dispatch_lat_us = [
        r['latency_us'] for r in per_requests
        if r['resp_type'] == 'dispatch' and r['latency_us'] is not None
    ]
    fallback_lat_us = [
        r['latency_us'] for r in per_requests
        if r['resp_type'] == 'fallback' and r['latency_us'] is not None
    ]

    total_sends = len(per_requests)
    successes = len(dispatch_lat_us)
    failures  = len(fallback_lat_us)
    timeouts  = total_sends - successes - failures

    def stats(arr):
        if not arr: 
            return {"count":0}
        return {
            "count": len(arr),
            "mean_us": statistics.mean(arr),
            "std_us": statistics.pstdev(arr) if len(arr)>1 else 0.0,
            "median_us": statistics.median(arr),
            "p99_us": sorted(arr)[max(0, math.ceil(0.99*len(arr))-1)]
        }

    return {
        "total_sends": total_sends,
        "successes": successes,
        "failures": failures,
        "timeouts": timeouts,
        "success_rate": successes / total_sends if total_sends>0 else 0.0,
        "dispatch_stats": stats(dispatch_lat_us),
        "fallback_stats": stats(fallback_lat_us),
    }

def main():
    csv_path = find_latest_rtl_csv()
    print("Parsing RTL CSV:", csv_path)
    events = load_events(csv_path)
    per_requests = pair_queue_style(events)

    # Save per-request CSV
    perreq_csv = OUT_DIR / "per_request_latencies.csv"
    with open(perreq_csv, "w", newline='') as f:
        w = csv.writer(f)
        w.writerow(["send_cycle","resp_type","resp_cycle","latency_cycles","latency_us","dispatch_slot"])
        for r in per_requests:
            w.writerow([
                r["send_cycle"],
                r["resp_type"] or "",
                r["resp_cycle"] or "",
                r["latency_cycles"] or "",
                (f"{r['latency_us']:.6f}" if r['latency_us'] is not None else ""),
                r["dispatch_slot"] if r["dispatch_slot"] is not None else ""
            ])

    summary = summarize(per_requests)
    summary_file = OUT_DIR / "rtl_metrics_summary.json"
    with open(summary_file, "w") as f:
        json.dump(summary, f, indent=2)

    print("Wrote per-request CSV to:", perreq_csv)
    print("Wrote summary JSON to:", summary_file)
    print(json.dumps(summary, indent=2))

if __name__ == "__main__":
    main()
