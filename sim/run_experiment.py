"""
run_experiment.py

Runs a single experiment and saves raw arrays + JSON summary into evaluation/.
"""

import json
import os
import time
from datetime import datetime
import numpy as np
from simulation_engine import LatencySimulator
import model_params as mp

OUTPUT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "evaluation/logs"))
os.makedirs(OUTPUT_DIR, exist_ok=True)


def run_and_save(num_requests: int = mp.NUM_REQUESTS,
                 seed: int = mp.RNG_SEED,
                 miss_rate: float = None,
                 paired: bool = True,
                 warmup: int = None,
                 fallback_full_baseline: bool = False):
    sim = LatencySimulator(seed=seed)
    if warmup is None:
        warmup = mp.WARMUP_REQUESTS

    print(f"Running simulation: requests={num_requests}, seed={seed}, miss_rate={miss_rate}, paired={paired}, warmup={warmup}")
    start = time.time()

    baseline, fastpath, miss_mask = sim.run_n_requests(num_requests, miss_rate=miss_rate, paired=paired,
                                                       warmup=warmup, fallback_full_baseline=fallback_full_baseline)

    elapsed = time.time() - start
    print(f"Simulation finished in {elapsed:.2f}s")

    stats = {
        "Baseline Mean": float(np.mean(baseline)),
        "Baseline Std": float(np.std(baseline)),
        "Baseline Median": float(np.median(baseline)),
        "Baseline P99": float(np.percentile(baseline, 99)),
        "FastPath Mean": float(np.mean(fastpath)),
        "FastPath Std": float(np.std(fastpath)),
        "FastPath Median": float(np.median(fastpath)),
        "FastPath P99": float(np.percentile(fastpath, 99)),
        "Observed Miss Fraction": float(np.mean(miss_mask)),
    }
    stats["Speedup (Mean)"] = stats["Baseline Mean"] / stats["FastPath Mean"]
    stats["Speedup (P99)"] = stats["Baseline P99"] / stats["FastPath P99"]

    timestamp = datetime.utcnow().strftime("%Y%m%dT%H%M%SZ")
    base_name = f"sim_results_{timestamp}"
    np.save(os.path.join(OUTPUT_DIR, base_name + "_baseline.npy"), baseline)
    np.save(os.path.join(OUTPUT_DIR, base_name + "_fastpath.npy"), fastpath)
    np.save(os.path.join(OUTPUT_DIR, base_name + "_missmask.npy"), miss_mask.astype(np.uint8))

    # Save JSON summary + metadata
    summary_path = os.path.join(OUTPUT_DIR, base_name + "_summary.json")
    meta = {
        "generated_at": datetime.utcnow().isoformat() + "Z",
        "num_requests": num_requests,
        "seed": seed,
        "miss_rate": miss_rate if miss_rate is not None else mp.CACHE_MISS_RATE,
        "paired": paired,
        "warmup": warmup,
        "fallback_full_baseline": fallback_full_baseline,
    }
    meta.update(mp.RUN_METADATA)
    with open(summary_path, "w") as f:
        json.dump({"meta": meta, "stats": stats}, f, indent=2)

    print("Summary stats:")
    for k, v in stats.items():
        if "Speedup" in k:
            print(f"  {k}: {v:.3f}x")
        else:
            print(f"  {k}: {v:.3f} us")
    print("Saved results to:", OUTPUT_DIR)
    return baseline, fastpath, miss_mask


if __name__ == "__main__":
    run_and_save()
