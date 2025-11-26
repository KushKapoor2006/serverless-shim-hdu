#!/usr/bin/env python3
"""
Comparison + plotting (fixed):
 - fixes 'local variable np referenced before assignment'
 - makes distribution plot robust for tiny / degenerate datasets
 - preserves original outputs: sw/logs/*.png and comparison_report.txt
"""
import json
import os
import re
import sys
from pathlib import Path

import numpy as np
import matplotlib.pyplot as plt
import csv

REPO_ROOT = Path(__file__).resolve().parents[2]
SW_LOG_DIR = REPO_ROOT / "sw" / "logs"
SW_LOG_DIR.mkdir(parents=True, exist_ok=True)
PERREQ_CSV = SW_LOG_DIR / "per_request_latencies.csv"
RTL_SUMMARY = SW_LOG_DIR / "rtl_metrics_summary.json"
SIM_LOG_DIR = REPO_ROOT / "evaluation" / "logs"

OUT_COMP = SW_LOG_DIR / "comparison_report.txt"
DIST_PLOT = SW_LOG_DIR / "distribution_plot.png"
BAR_PLOT = SW_LOG_DIR / "comparison_bars.png"

SIM_PATTERN = re.compile(r"sim_results_(\d{8}T\d{6}Z)_(baseline|fastpath|missmask)\.npy")
SUM_PATTERN = re.compile(r"sim_results_(\d{8}T\d{6}Z)_summary\.json")

# Default model params if we cannot import sim/model_params.py
DEFAULT_NIC_MEAN_US = 1.5
DEFAULT_PCIE_DIRECT_RX_MEAN_US = 2.5

def find_latest_sim_timestamp():
    timestamps = []
    if not SIM_LOG_DIR.exists():
        return None
    for f in SIM_LOG_DIR.iterdir():
        m1 = SIM_PATTERN.match(f.name)
        m2 = SUM_PATTERN.match(f.name)
        if m1:
            timestamps.append(m1.group(1))
        elif m2:
            timestamps.append(m2.group(1))
    return max(timestamps) if timestamps else None

def load_sim_results(ts):
    baseline = None
    fastpath = None
    stats = {}
    if ts:
        bf = SIM_LOG_DIR / f"sim_results_{ts}_baseline.npy"
        ff = SIM_LOG_DIR / f"sim_results_{ts}_fastpath.npy"
        sj = SIM_LOG_DIR / f"sim_results_{ts}_summary.json"
        if bf.exists():
            baseline = np.load(str(bf))
            stats['baseline_mean_us'] = float(np.mean(baseline))
            stats['baseline_p99_us'] = float(np.percentile(baseline,99))
        if ff.exists():
            fastpath = np.load(str(ff))
            stats['fast_mean_us'] = float(np.mean(fastpath))
            stats['fast_p99_us'] = float(np.percentile(fastpath,99))
        if sj.exists():
            with open(sj) as f:
                summary = json.load(f)
            stats.update(summary)
    return stats, baseline, fastpath

def load_rtl_perreqs():
    """Load per-request CSV created by parse_rtl_logs.py"""
    rows = []
    if not PERREQ_CSV.exists():
        return rows
    with open(PERREQ_CSV, newline='') as f:
        r = csv.DictReader(f)
        for row in r:
            rows.append(row)
    return rows

def extract_rtl_dispatch_latencies(rows):
    lat_us = []
    for r in rows:
        if r.get("resp_type") == "dispatch" and r.get("latency_us"):
            try:
                lat_us.append(float(r["latency_us"]))
            except:
                pass
    return np.array(lat_us)

def plot_distributions(baseline, fastpath, rtl_internal, rtl_full, outpath):
    plt.figure(figsize=(9,5))
    datasets = []
    labels = []
    alphas = []

    if baseline is not None and getattr(baseline, "size", 0) > 0:
        datasets.append(np.asarray(baseline).ravel())
        labels.append('SIM Baseline')
        alphas.append(0.35)
    if fastpath is not None and getattr(fastpath, "size", 0) > 0:
        datasets.append(np.asarray(fastpath).ravel())
        labels.append('SIM FastPath')
        alphas.append(0.35)
    if rtl_full is not None and getattr(rtl_full, "size", 0) > 0:
        datasets.append(np.asarray(rtl_full).ravel())
        labels.append('RTL FastPath (adjusted)')
        alphas.append(0.6)
    if rtl_internal is not None and getattr(rtl_internal, "size", 0) > 0:
        datasets.append(np.asarray(rtl_internal).ravel())
        labels.append('RTL HDU internal')
        alphas.append(0.6)

    if not datasets:
        plt.text(0.5,0.5,"No data available for distribution plot", ha='center')
        plt.xlabel('Latency (µs)')
        plt.ylabel('Density')
        plt.title('Latency Distributions (SIM vs RTL)')
        plt.grid(alpha=0.3)
        plt.tight_layout()
        plt.savefig(outpath, dpi=200)
        plt.close()
        return

    combined_all = np.concatenate(datasets)
    minv, maxv = float(np.min(combined_all)), float(np.max(combined_all))

    # Handle degenerate / single-value datasets
    if np.isclose(minv, maxv):
        spread = max(1e-6, abs(minv)*0.01 + 1e-3)
        min_edge = max(0.0, minv - spread)
        max_edge = maxv + spread
    else:
        min_edge = max(0.0, minv)
        max_edge = maxv

    # Choose bins robustly
    bins = 100
    if combined_all.size < 50:
        bins = max(10, int(max(10, combined_all.size // 2)))

    for data, label, alpha in zip(datasets, labels, alphas):
        plt.hist(data, bins=bins, range=(min_edge, max_edge), alpha=alpha, label=label, density=True)

    plt.xlabel('Latency (µs)')
    plt.ylabel('Density')
    plt.title('Latency Distributions (SIM vs RTL)')
    plt.legend()
    plt.grid(alpha=0.3)
    xpad = (max_edge - min_edge) * 0.05 if (max_edge - min_edge) > 0 else max(0.1, max_edge*0.05)
    plt.xlim(max(0.0, min_edge - xpad), max_edge + xpad)
    plt.tight_layout()
    plt.savefig(outpath, dpi=200)
    plt.close()

def plot_bars(sim_stats, rtl_summary, rtl_full, outpath):
    # DO NOT import numpy here (would create a local 'np' which breaks earlier np usage)
    sim_mean = sim_stats.get("fast_mean_us") if sim_stats else None
    sim_p99 = sim_stats.get("fast_p99_us") if sim_stats else None

    rtl_internal_mean = rtl_summary.get("dispatch_stats", {}).get("mean_us")
    rtl_internal_p99  = rtl_summary.get("dispatch_stats", {}).get("p99_us")

    rtl_full_mean = float(np.mean(rtl_full)) if (rtl_full is not None and rtl_full.size>0) else None
    rtl_full_p99  = float(np.percentile(rtl_full, 99)) if (rtl_full is not None and rtl_full.size>0) else None

    categories = ['Mean (µs)', 'P99 (µs)']
    sim_vals = [sim_mean or 0.0, sim_p99 or 0.0]
    rtl_vals = [rtl_full_mean or 0.0, rtl_full_p99 or 0.0]

    x = np.arange(len(categories))
    width = 0.35

    fig, ax = plt.subplots(figsize=(7,4))
    ax.bar(x - width/2, sim_vals, width, label='SIM FastPath')
    ax.bar(x + width/2, rtl_vals, width, label='RTL FastPath (adjusted)')

    for i, v in enumerate(sim_vals):
        ax.text(i - width/2, v + max(1e-6, 0.005*max(1.0,v)), f"{v:.4g}", ha='center', va='bottom', fontsize=8)
    for i, v in enumerate(rtl_vals):
        ax.text(i + width/2, v + max(1e-6, 0.005*max(1.0,v)), f"{v:.4g}", ha='center', va='bottom', fontsize=8)

    ax.set_xticks(x)
    ax.set_xticklabels(categories)
    ax.set_ylabel('Latency (µs)')
    ax.set_title('SIM vs RTL: Mean and P99 Comparison (Adjusted)')
    ax.legend()
    ax.grid(axis='y', alpha=0.3)
    fig.tight_layout()
    fig.savefig(outpath, dpi=200)
    plt.close()

def get_nic_pcie_means():
    try:
        sys.path.insert(0, str(REPO_ROOT / "sim"))
        import model_params as mp
        nic = float(mp.params_fastpath.get('nic_ingress', (DEFAULT_NIC_MEAN_US, 0.0))[0])
        pcie = float(mp.params_fastpath.get('pcie_direct_rx', (DEFAULT_PCIE_DIRECT_RX_MEAN_US,0.0))[0])
        return nic, pcie
    except Exception:
        return DEFAULT_NIC_MEAN_US, DEFAULT_PCIE_DIRECT_RX_MEAN_US
    finally:
        try:
            sys.path.pop(0)
        except:
            pass

def main():
    rows = load_rtl_perreqs()
    if not rows:
        print("No per-request CSV found at", PERREQ_CSV)
    rtl_internal = extract_rtl_dispatch_latencies(rows)

    ts = find_latest_sim_timestamp()
    sim_stats, baseline, fastpath = None, None, None
    if ts:
        sim_stats, baseline, fastpath = load_sim_results(ts)
        print("Using simulation timestamp:", ts)
    else:
        print("No simulation runs found under evaluation/logs")

    nic_mean, pcie_mean = get_nic_pcie_means()
    print(f"NIC mean = {nic_mean} us, PCIe direct rx mean = {pcie_mean} us (used to adjust RTL)")

    if rtl_internal is not None and rtl_internal.size>0:
        rtl_full = rtl_internal + (nic_mean + pcie_mean)
    else:
        rtl_full = np.array([])

    # write report
    with open(OUT_COMP, "w") as f:
        f.write("=== Comparison Report ===\n\n")
        if rtl_internal is not None:
            f.write(f"RTL internal dispatch count: {len(rtl_internal)}\n")
        f.write("\nRTL summary (sw/logs/rtl_metrics_summary.json):\n")
        if RTL_SUMMARY.exists():
            with open(RTL_SUMMARY) as rf:
                rtl_summary = json.load(rf)
            json.dump(rtl_summary, f, indent=2)
        else:
            f.write("RTL summary JSON not found.\n")
        f.write("\n\n")
        if ts and sim_stats:
            f.write("SIM summary (latest timestamp: {})\n".format(ts))
            json.dump(sim_stats, f, indent=2)
            f.write("\n\n")
            if "fast_mean_us" in sim_stats and rtl_full.size>0:
                f.write(f"SIM fast_mean_us: {sim_stats['fast_mean_us']:.6f} us\n")
                f.write(f"RTL adjusted mean (nic+pcie + internal): {float(np.mean(rtl_full)):.6f} us\n")
                f.write(f"SIM/RTL speedup: {sim_stats['fast_mean_us']/float(np.mean(rtl_full)):.3f}x\n")
            else:
                f.write("Not enough data to compare means.\n")
        else:
            f.write("No SIM stats loaded for comparison.\n")

    print("Wrote comparison report to", OUT_COMP)

    # plots
    try:
        plot_distributions(baseline, fastpath, rtl_internal, rtl_full, DIST_PLOT)
        print("Wrote distribution plot to", DIST_PLOT)
    except Exception as e:
        print("Failed to write distribution plot:", e)

    try:
        # load rtl_summary again for bar plot
        rtl_summary = {}
        if RTL_SUMMARY.exists():
            with open(RTL_SUMMARY) as rf:
                rtl_summary = json.load(rf)
        plot_bars(sim_stats or {}, rtl_summary, rtl_full, BAR_PLOT)
        print("Wrote bar plot to", BAR_PLOT)
    except Exception as e:
        print("Failed to write bar plot:", e)

if __name__ == "__main__":
    main()
