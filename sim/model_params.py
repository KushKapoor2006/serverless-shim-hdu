"""
model_params.py
Configuration parameters for the simulation.

All time units are in microseconds (us).
This file is aligned to the RTL hdu_pkg defaults:
  - Clock: 200 MHz (5 ns per cycle)
  - HDU parse/lookup/alloc/dispatch cycles are taken from hdu_pkg:
      HDU_PARSE_CYCLES    = 20   -> 20 * 0.005 us = 0.1 us
      HDU_LOOKUP_CYCLES   = 30   -> 30 * 0.005 us = 0.15 us
      HDU_ALLOC_CYCLES    = 10   -> 10 * 0.005 us = 0.05 us
      HDU_DISPATCH_CYCLES = 20   -> 20 * 0.005 us = 0.1 us

If you change cycle counts in RTL, update these values to keep Python/Rtl parity.
"""

from datetime import datetime

# -------------------------
# Simulation control
# -------------------------
NUM_REQUESTS = 10_000
WARMUP_REQUESTS = 1_000

# RNG seed (None => non-deterministic). Set to an int for reproducible runs.
RNG_SEED = 12345

# Small epsilon to clamp negative-sampled latencies without biasing small means.
EPS = 1e-9

# -------------------------
# Clock / cycle mapping (match RTL hdu_pkg)
# -------------------------
CLK_FREQ_MHZ = 200                      # 200 MHz RTL clock
CYCLE_USEC = 1.0 / (CLK_FREQ_MHZ * 1000.0)  # microseconds per cycle (0.005 us)

# HDU cycle counts (must match hdu_pkg)
HDU_PARSE_CYCLES    = 20
HDU_LOOKUP_CYCLES   = 30
HDU_ALLOC_CYCLES    = 10
HDU_DISPATCH_CYCLES = 20

# Convenience: convert cycles -> microseconds (float)
def cycles_to_usec(cycles: int) -> float:
    return cycles * CYCLE_USEC

# -------------------------
# RTL/Hardware-related params (for parity with RTL)
# -------------------------
CAM_TABLE_SIZE = 16    # Should match auth_cam TABLE_SIZE parameter in RTL
MAX_SLOTS = 32         # Should match MAX_SLOTS in RTL

# Host-side interrupt handling latency used when modeling fallback penalty.
# In RTL the fallback_bridge dequeues to host depending on host_ready; here we
# model that with a fixed host interrupt + host processing penalty (in us).
HOST_INTERRUPT_LATENCY = 2.0  # microseconds (tunable; TTL overhead to notify host)
HOST_PROCESSING_LATENCY = 50.0  # approximate full host path processing (us)
# Note: total fallback penalty in the model = HOST_INTERRUPT_LATENCY + HOST_PROCESSING_LATENCY
# You can tune HOST_PROCESSING_LATENCY to reflect actual host behavior or TB model.

# -------------------------
# SIMULATION "PHYSICS"
# -------------------------
# HOST SYSTEM LATENCIES (The "Slow Path")
# These are conservative model numbers; units are microseconds.
params_baseline = {
    'nic_ingress': (1.5, 0.2),
    'pcie_host_rx': (2.0, 0.5),
    'driver_interrupt': (5.0, 2.0),
    'os_network_stack': (15.0, 5.0),
    'middleware_logic': (20.0, 8.0),
    'driver_tx_setup': (5.0, 1.0),
    'pcie_xpu_tx': (2.0, 0.5),
}

# HARDWARE DISPATCHER LATENCIES (The "Fast Path")
# The deterministic HDL latencies are derived from RTL cycle counts above.
params_fastpath = {
    'nic_ingress': (1.5, 0.2),
    'pcie_direct_rx': (2.5, 0.5),
    # Map RTL cycle counts -> microseconds for precise parity
    'hdu_parse': (cycles_to_usec(HDU_PARSE_CYCLES), 0.0),    # deterministic in RTL
    'hdu_lookup': (cycles_to_usec(HDU_LOOKUP_CYCLES), 0.0),  # deterministic in RTL
    'hdu_alloc': (cycles_to_usec(HDU_ALLOC_CYCLES), 0.0),    # deterministic in RTL
    'hdu_dispatch': (cycles_to_usec(HDU_DISPATCH_CYCLES), 0.0), # deterministic in RTL
}

# Default miss rate (can be overridden at runtime)
# NOTE: Miss rate depends on CAM_TABLE_SIZE and workload diversity.
# For RTL parity, set this to reflect CAM occupancy & workload (e.g., 0.05).
CACHE_MISS_RATE = 0.05  # 5% default; sweep during experiments

# -------------------------
# Metadata helper for experiment files
# -------------------------
RUN_METADATA = {
    "created_at": datetime.utcnow().isoformat() + "Z",
    "notes": "RTL-aligned timings; units = microseconds; clock=200MHz",
    "clk_freq_mhz": CLK_FREQ_MHZ,
    "hdu_cycles": {
        "parse": HDU_PARSE_CYCLES,
        "lookup": HDU_LOOKUP_CYCLES,
        "alloc": HDU_ALLOC_CYCLES,
        "dispatch": HDU_DISPATCH_CYCLES
    },
    "cam_table_size": CAM_TABLE_SIZE,
    "max_slots": MAX_SLOTS
}
