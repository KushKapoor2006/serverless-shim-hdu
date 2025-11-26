"""
analysis_tools.py

Small utilities for bootstrap confidence intervals and basic diagnostics.
"""

import numpy as np
from typing import Tuple


def bootstrap_ci(data: np.ndarray, stat_fn, n_resamples: int = 2000, alpha: float = 0.05, rng=None) -> Tuple[float, float]:
    """
    Compute (alpha/2, 1-alpha/2) bootstrap CI for statistic defined by stat_fn.
    stat_fn takes a 1D array and returns a scalar.
    """
    rng = np.random.default_rng() if rng is None else rng
    n = len(data)
    res = np.empty(n_resamples, dtype=float)
    for i in range(n_resamples):
        idx = rng.integers(0, n, size=n)
        sample = data[idx]
        res[i] = stat_fn(sample)
    lower = np.percentile(res, 100 * (alpha / 2.0))
    upper = np.percentile(res, 100 * (1 - alpha / 2.0))
    return lower, upper


def percentile_stat(p: float):
    return lambda arr: float(np.percentile(arr, p))
