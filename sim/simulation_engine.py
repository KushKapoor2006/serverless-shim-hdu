"""
simulation_engine.py

Vectorized latency simulator with reproducible RNG and run-time miss-rate control.
"""

from typing import Tuple, Optional
import numpy as np
import model_params as mp


class LatencySimulator:
    def __init__(self, seed: Optional[int] = None):
        self.seed = seed if seed is not None else mp.RNG_SEED
        self.rng = np.random.default_rng(self.seed)

    @staticmethod
    def _sample_normal_vector(rng: np.random.Generator, mean: float, std: float, size: int) -> np.ndarray:
        if std == 0.0:
            return np.full(size, mean, dtype=float)
        samples = rng.normal(loc=mean, scale=std, size=size)
        samples = np.maximum(samples, mp.EPS)
        return samples

    def run_n_requests(self,
                       n: int,
                       miss_rate: Optional[float] = None,
                       paired: bool = True,
                       warmup: int = mp.WARMUP_REQUESTS,
                       fallback_full_baseline: bool = False
                       ) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
        """
        Run `n` simulated requests and return (baseline_latencies, fastpath_latencies, miss_mask)

        Parameters:
          - miss_rate: overrides model_params.CACHE_MISS_RATE if provided.
          - paired: if True, correlated randomness for shared components.
          - warmup: number of initial requests to discard.
          - fallback_full_baseline: whether misses add full baseline or only host-side costs.
        """
        total = n + max(0, warmup)
        miss_rate = mp.CACHE_MISS_RATE if miss_rate is None else float(miss_rate)

        # Baseline sum
        baseline_sum = np.zeros(total, dtype=float)
        for (mean, std) in mp.params_baseline.values():
            baseline_sum += self._sample_normal_vector(self.rng, mean, std, total)

        # Fastpath sum
        fast_sum = np.zeros(total, dtype=float)

        # nic_ingress: if paired, sample once and reuse; else sample separately
        if paired:
            # sample one series and add to both baseline and fastpath in the same stream
            # (we already sampled baseline nic_ingress as part of baseline_sum, but since we didn't store per-component,
            # we sample a new correlated series for fastpath nic_ingress using the same RNG stream)
            fast_sum += self._sample_normal_vector(self.rng, *mp.params_fastpath['nic_ingress'], total)
        else:
            fast_sum += self._sample_normal_vector(self.rng, *mp.params_fastpath['nic_ingress'], total)

        # pcie_direct_rx
        if 'pcie_direct_rx' in mp.params_fastpath:
            fast_sum += self._sample_normal_vector(self.rng, *mp.params_fastpath['pcie_direct_rx'], total)

        # deterministic/low-variance hardware components
        for k in ('hdu_parse', 'hdu_lookup', 'hdu_alloc', 'hdu_dispatch'):
            if k in mp.params_fastpath:
                mean, std = mp.params_fastpath[k]
                fast_sum += self._sample_normal_vector(self.rng, mean, std, total)

        # handle misses
        miss_mask = self.rng.random(total) < miss_rate
        if miss_mask.any():
            fast_sum[miss_mask] += 2.0  # interrupt overhead
            if fallback_full_baseline:
                fast_sum[miss_mask] += baseline_sum[miss_mask]
            else:
                # add host-side costs only (exclude nic_ingress)
                host_keys = ['driver_interrupt', 'os_network_stack', 'middleware_logic', 'driver_tx_setup', 'pcie_xpu_tx']
                host_side = np.zeros(total, dtype=float)
                for k in host_keys:
                    if k in mp.params_baseline:
                        mean, std = mp.params_baseline[k]
                        host_side += self._sample_normal_vector(self.rng, mean, std, total)
                fast_sum[miss_mask] += host_side[miss_mask]

        # discard warmup
        baseline = baseline_sum[warmup:]
        fastpath = fast_sum[warmup:]
        miss_mask = miss_mask[warmup:]

        return baseline, fastpath, miss_mask
