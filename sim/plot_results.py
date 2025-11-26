"""
plot_analysis.py

Create multiple plots from the latest simulation run (or specific files).
 - Overlaid histograms
 - CDF (complementary)
 - Miss-only histogram
 - Fraction above latency thresholds
"""

import os
import glob
import numpy as np
import matplotlib.pyplot as plt

EVAL_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "evaluation/logs"))
OUT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "evaluation/plots"))
os.makedirs(OUT_DIR, exist_ok=True)



def latest_files():
    baselines = sorted(glob.glob(os.path.join(EVAL_DIR, "sim_results_*_baseline.npy")))
    if not baselines:
        raise FileNotFoundError("No sim_results files found in evaluation/; run run_experiment.py first.")
    baseline = baselines[-1]
    fastpath = baseline.replace("_baseline.npy", "_fastpath.npy")
    missmask = baseline.replace("_baseline.npy", "_missmask.npy")
    return baseline, fastpath, missmask


def plot_histograms(baseline, fastpath, outpath):
    plt.figure(figsize=(10, 6))
    plt.hist(baseline, bins=200, alpha=0.5, label='Baseline', density=True)
    plt.hist(fastpath, bins=200, alpha=0.6, label='FastPath', density=True)
    plt.xlabel('Latency (us)')
    plt.ylabel('Density')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(outpath, dpi=300)
    print("Saved histogram:", outpath)


def plot_cdf(baseline, fastpath, outpath):
    def cdf(data):
        s = np.sort(data)
        y = np.arange(1, len(s)+1) / len(s)
        return s, y

    bs, by = cdf(baseline)
    fs, fy = cdf(fastpath)

    plt.figure(figsize=(10, 6))
    plt.plot(bs, by, label='Baseline CDF')
    plt.plot(fs, fy, label='FastPath CDF')
    plt.xlabel('Latency (us)')
    plt.ylabel('CDF')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(outpath, dpi=300)
    print("Saved CDF:", outpath)


def plot_miss_histogram(fastpath, missmask, outpath):
    misses = fastpath[missmask.astype(bool)]
    hits = fastpath[~missmask.astype(bool)]

    plt.figure(figsize=(10, 6))
    plt.hist(hits, bins=200, alpha=0.6, label='FastPath Hits', density=True)
    plt.hist(misses, bins=200, alpha=0.6, label='FastPath Misses', density=True)
    plt.xlabel('FastPath Latency (us)')
    plt.ylabel('Density')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.savefig(outpath, dpi=300)
    print("Saved miss-histogram:", outpath)


def plot_threshold_fraction(baseline, fastpath, outpath):
    thresholds = [1, 2, 5, 10, 20, 50, 100]
    results = []
    for t in thresholds:
        results.append({
            'threshold': t,
            'baseline_fraction': np.mean(baseline > t),
            'fast_fraction': np.mean(fastpath > t)
        })
    # plot
    plt.figure(figsize=(8, 5))
    th = [r['threshold'] for r in results]
    base_frac = [r['baseline_fraction'] for r in results]
    fast_frac = [r['fast_fraction'] for r in results]
    plt.plot(th, base_frac, marker='o', label='Baseline')
    plt.plot(th, fast_frac, marker='o', label='FastPath')
    plt.xscale('log')
    plt.xlabel('Latency Threshold (us, log scale)')
    plt.ylabel('Fraction > threshold')
    plt.title('Tail fractions')
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(outpath, dpi=300)
    print("Saved threshold fractions:", outpath)


if __name__ == "__main__":
    baseline_path, fastpath_path, missmask_path = latest_files()
    baseline = np.load(baseline_path)
    fastpath = np.load(fastpath_path)
    missmask = np.load(missmask_path)

    plot_histograms(baseline, fastpath, os.path.join(OUT_DIR, "histogram_baseline_fastpath.png"))
    plot_cdf(baseline, fastpath, os.path.join(OUT_DIR, "cdf_baseline_fastpath.png"))
    plot_miss_histogram(fastpath, missmask, os.path.join(OUT_DIR, "hist_miss_vs_hit.png"))
    plot_threshold_fraction(baseline, fastpath, os.path.join(OUT_DIR, "threshold_fractions.png"))
