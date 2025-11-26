"""
sweep_missrate.py

Sweep CACHE_MISS_RATE across a list of values, run the simulation for each,
and save aggregated statistics into evaluation/sweep_missrate.csv.
Also creates a 'speedup_vs_missrate.png'.
"""

import csv
import os
import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime
from run_experiment import run_and_save
import model_params as mp

OUTPUT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "evaluation/logs"))
OUTPUT_DIR2 = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "evaluation/plots"))
os.makedirs(OUTPUT_DIR, exist_ok=True)
os.makedirs(OUTPUT_DIR2, exist_ok=True)

MISS_VALUES = [0.0005, 0.001, 0.002, 0.005, 0.01, 0.02, 0.05]  # from 0.05% to 5%
REPEATS = 3  # run multiple seeds for variance estimate; keep small for speed

def sweep():
    rows = []
    for miss in MISS_VALUES:
        for r in range(REPEATS):
            seed = mp.RNG_SEED + r  # varied seed
            baseline, fastpath, miss_mask = run_and_save(num_requests=mp.NUM_REQUESTS,
                                                         seed=seed,
                                                         miss_rate=miss,
                                                         paired=True,
                                                         warmup=mp.WARMUP_REQUESTS,
                                                         fallback_full_baseline=False)
            row = {
                "miss_rate": miss,
                "seed": seed,
                "observed_miss_fraction": float(np.mean(miss_mask)),
                "baseline_mean": float(np.mean(baseline)),
                "baseline_p99": float(np.percentile(baseline, 99)),
                "fast_mean": float(np.mean(fastpath)),
                "fast_p99": float(np.percentile(fastpath, 99)),
                "speedup_mean": float(np.mean(baseline)) / float(np.mean(fastpath)),
                "speedup_p99": float(np.percentile(baseline, 99)) / float(np.percentile(fastpath, 99))
            }
            rows.append(row)

    csv_path = os.path.join(OUTPUT_DIR, f"sweep_missrate_{datetime.utcnow().strftime('%Y%m%dT%H%M%SZ')}.csv")
    with open(csv_path, "w", newline='') as csvfile:
        fieldnames = list(rows[0].keys())
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for r in rows:
            writer.writerow(r)

    # quick plot: speedup_mean vs miss_rate
    # aggregate by miss_rate (mean across repeats)
    import pandas as pd
    df = pd.DataFrame(rows)
    agg = df.groupby('miss_rate').agg({
        'speedup_mean': ['mean', 'std'],
        'speedup_p99': ['mean', 'std'],
        'fast_p99': ['mean']
    }).reset_index()
    # flatten columns
    agg.columns = ['miss_rate', 'speedup_mean_mean', 'speedup_mean_std', 'speedup_p99_mean', 'speedup_p99_std', 'fast_p99_mean']

    plt.figure(figsize=(8, 5))
    plt.errorbar(agg['miss_rate'], agg['speedup_mean_mean'], yerr=agg['speedup_mean_std'], marker='o', label='Speedup (Mean)')
    plt.xscale('log')
    plt.xlabel('Miss Rate (fraction, log scale)')
    plt.ylabel('Speedup (mean)')
    plt.title('Speedup (Mean) vs Miss Rate')
    plt.grid(True, alpha=0.3)
    plt.legend()
    png = os.path.join(OUTPUT_DIR2, "speedup_vs_missrate.png")
    plt.savefig(png, dpi=300)
    print(f"Sweep CSV written to {csv_path}")
    print(f"Plot saved to {png}")

if __name__ == "__main__":
    sweep()
