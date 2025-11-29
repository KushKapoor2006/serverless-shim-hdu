# Serverless HDU — Research README

> **Project (research internship / research assistant work)** — Hardware Dispatch Unit (HDU) to accelerate serverless-style request dispatching. Synthesizable SystemVerilog RTL, cycle-accurate Verilator harnesses, Python SIM model and analysis pipeline, and an open-source synthesis flow (Yosys).

---

## TL;DR

A compact HDU microarchitecture (CAM-based authorization, bitmap slot allocator, fallback FIFO) implemented in SystemVerilog, validated with Verilator and a Python SIM model. End-to-end evaluation (SIM ↔ RTL) and open-source synthesis (Yosys) are included. Key results from the included runs:

* SIM (10k requests): **FastPath mean ≈ 6.83 µs**, **Baseline mean ≈ 50.60 µs**, **mean speedup ≈ 7.41×**.
* RTL (Verilator small TB): internal dispatch mean **0.02 µs**; RTL-adjusted end-to-end mean ≈ **4.02 µs** (after adding NIC+PCIe means).
* Synthesis (Yosys, flattened): **Number of cells: 425**, **DFFs: 32**, **Mux cells: 279**, **Number of wires: 441 (5,436 wire bits)**.

These artifacts are reproducible using the scripts and Makefiles in the repository.

---

## Table of contents

1. [Overview & Motivation](#overview--motivation)
2. [Project Summary & Contributions](#project-summary--contributions)
3. [Repository layout (high-level)](#repository-layout-high-level)
4. [Quickstart & execution pipeline (reproducible)](#quickstart--execution-pipeline-reproducible)
5. [Design & RTL modules](#design--rtl-modules)
6. [Simulation & analysis pipeline (method)](#simulation--analysis-pipeline-method)
7. [Results & figures included](#results--figures-included)
8. [Synthesis & how to reproduce area metrics](#synthesis--how-to-reproduce-area-metrics)
9. [Files with Makefiles and commands](#files-with-makefiles-and-commands)
11. [Supervision & authorship](#supervision--authorship)
12. [Limitations & future work](#limitations--future-work)
13. [License & contact](#license--contact)

---

## Overview & Motivation

Serverless workloads (event-driven functions) are extremely latency-sensitive. Software dispatching adds overhead across NIC, kernel and userspace. The HDU offloads authorization and slot allocation to hardware (fast-path), providing a fallback path for misses. The project explores this trade-off with a paired SIM model and RTL verification to produce reproducible latency and area claims.

---

## Project Summary & Contributions

* Implemented a synthesizable HDU in SystemVerilog: **auth_cam**, **slot_allocator**, **fallback_bridge**, and `hdu_top` glue.
* Built a reproducible evaluation pipeline: Python SIM model (10k-request experiments), Verilator TBs, RTL log parser, and comparison/plotting scripts.
* Open-source synthesis flow using Yosys to generate hierarchical and flattened netlists + technology-neutral area metrics.
* Produced reproducible artifacts (SIM JSONs, RTL CSV logs, plots, Yosys outputs) suitable for CV/academic submission.

---

## Repository layout (high-level)

Use this for quick navigation in the repo (trimmed to one level):

```text
serverless/
├─ sim/                    # High level python simulations
│
├─ hw/                     # RTL simulation
│  ├─ rtl/
│  ├─ Makefile             # Verilator build + run targets
│  ├─ verilator_harness.cpp
│  └─ obj_dir/             # Verilator-generated C++ + build artifacts
│
├─ sw/                     # Hardware-Software Co-Simulation
│  ├─ driver/
│  ├─ tests/
│  ├─ logs/                # Graphs, CSV, JSON
│  └─ Makefile             # sw-side helper targets
│
├─ synthesis/              # Yosys
│  ├─ hdu_synth.ys
│  ├─ rtl/                 # Yosys-friendly copy of RTL
│  └─ outputs/
│
├─ evaluation/             # Results of Simulation
│  ├─ logs/
│  └─ plots/
└─ README.md
```

Notes:

* `hw/rtl` contains the SystemVerilog sources used for simulation.
* `synthesis/rtl` contains a Yosys-friendly copy (`hdu_defs.svh` etc.) used for synthesis.
* `sw/logs` and `evaluation/plots` hold plots and processed outputs included for quick review.

---

## Quickstart & execution pipeline (reproducible)

This section lists the canonical sequence to reproduce the major artifacts: SIM results, RTL log parsing, comparison plots, and synthesis outputs. All commands assume you are in the repo root and required tools are installed.

### Prerequisites

* Python 3.8+ with `numpy`, `matplotlib`, `pandas` (optional).
* Verilator (for RTL TB)
* Yosys (for synthesis)
* Graphviz (optional, for DOT → PDF/PNG)

Install Python deps quickly:

```bash
pip3 install numpy matplotlib pandas
```

### 1) RTL verification (Verilator TB)

Build and run the Verilator testbench. The `hw/Makefile` provides convenient targets.

```bash
cd hw
make run
# Outputs: evaluation/logs/rtl_run_*.csv  (the harness also writes per-request logs)
cd ..
```

The TB writes event timelines to `evaluation/logs/rtl_run_*.csv` which the parser consumes.

### 2) Parse RTL logs in Software side (per-request latencies)

```bash
cd sw
make run
python3 sw/tests/parse_rtl_logs.py
# outputs: sw/logs/per_request_latencies.csv, sw/logs/rtl_metrics_summary.json
```

The parser implements queue-based pairing of `send_packet` → (`dispatch_slot_*` | `auth_fallback`).

### 3) Run SIM experiments (large-scale)

```bash
python3 sim/run_experiment.py
# outputs: evaluation/logs/sim_results_{TS}_*.npy and sim_results_{TS}_summary.json
```

Defaults: 10k requests, seed in `sim/model_params.py`. Use `sweep_missrate.py` to evaluate miss-rate sensitivity.

### 4) Compare SIM vs RTL and generate plots

```bash
python3 sw/tests/compare_with_sim.py
# outputs: sw/logs/comparison_report.txt, plots in sw/logs/ and evaluation/plots/
```

This script loads the latest `sim_results_*` and the `sw/logs/per_request_latencies.csv`, adds NIC+PCIe means (from `sim/model_params`), and produces distribution/CDF/histogram plots.

### 5) Synthesis (Yosys)

```bash
cd synthesis
yosys -l outputs/hdu_synth.log -s hdu_synth.ys
# outputs: synthesis/outputs/hdu_top_flat.json, hdu_top_flat.v, hdu_synth.log
cd ..
```

The Yosys flow writes hierarchical and flattened netlists + `stat` blocks; inspect `synthesis/outputs/hdu_synth.log` and the flattened `=== hdu_top ===` block for the area metrics used in this README.

---

## Design & RTL modules

(Short summary plus key signals and behaviors. See RTL files for full details.)

### `hdu_defs.svh` / `hdu_pkg.sv` (Yosys vs legacy)

* Project-wide constants and widths. `hdu_defs.svh` is a Yosys-friendly include that defines macros such as `HDU_DATA_WIDTH`, `HDU_FUNC_ID_WIDTH`, `HDU_TOKEN_WIDTH`, `HDU_SLOT_ID_WIDTH` and header bit positions. The legacy `hdu_pkg.sv` (if present) contains the equivalent typedefs/parameters used during simulation; for synthesis prefer the `.svh` include.

### `header_parser.sv`

* Extracts fields from the input header word: `func_id` and `token`. Handles simple input-valid latching and presents `out_valid`, `out_func_id`, and `out_token` to downstream blocks.
* Key signals: `in_valid`, `in_data[DATA_W-1:0]`, `out_valid`, `out_func_id`, `out_token`.

### `auth_cam.sv`

* Small content-addressable table implemented via registers. Supports configuration writes (`cfg_wr_en`, `cfg_addr`, `cfg_func_id`, `cfg_token`) and single-cycle lookups (`lookup_func_id`, `lookup_token` → `auth_pass`, `auth_done`).
* CAM modeled as parallel arrays (`table_func[]`, `table_tok[]`); lookups scan the table and assert `auth_pass` when a match is found.
* Key signals: `cfg_*` config interface, `lookup_*` lookup interface, `auth_pass`, `auth_done`.

### `slot_allocator.sv`

* Bitmap-based first-free allocator. Maintains a `busy` bitmask for `MAX_SLOTS` and returns the first free slot on request (`req_valid`). Also accepts frees (`free_en`, `free_slot_id`). Designed for minimal control logic and single-cycle readiness (where possible).
* Tracks allocation success/failure via `alloc_success`, `alloc_fail`, and `alloc_valid` signals.
* Key signals: `req_valid`, `req_ready`, `free_slot_id`, `free_en`, `alloc_slot_id`, `alloc_success`, `alloc_fail`, `alloc_valid`.

### `fallback_bridge.sv`

* Small FIFO that packs fallback information (function ID + truncated token) into a 64-bit entry for host consumption. Accepts `fail_valid`/`fail_*` and exposes `irq_valid`/`irq_data` to the host side.
* Internal circular buffer with `wr_ptr`, `rd_ptr`, `count`; reports overflows via a telemetry counter.
* Key signals: `fail_valid`, `fail_func_id`, `fail_token`, `host_ready`, `irq_valid`, `irq_data`.

### `hdu_top.sv`

* Top-level HDU that stitches together the header parser, auth CAM, slot allocator, and fallback bridge. Provides a test-harness injection port (`tb_inject_*`) for simulation, an AXI-Stream-like `s_axis_tdata/tvalid/tready` input, and dispatch outputs to compute (`dispatch_slot`, `dispatch_valid`).
* Exposes telemetry counters and host IRQ interface.
* Key behaviors: choose between TB-inject and AXIS input, issue lookup to CAM, on `auth_pass` request allocator and assert dispatch outputs; on miss, assert fallback.

### `verilator_harness.cpp` (hw/verilator harness)

* Drives clock/reset, packs test vectors into the DUT inputs, and logs timestamped events to produce `evaluation/logs/rtl_run_*.csv` with lines `event,cycle,ns`.
* Emits per-request logs (`verilator_run_log.txt`) and optional VCD waveform dumps for detailed debugging.
* Used in `hw/Makefile` to build and run the TB.

### `sw/tests/parse_rtl_logs.py`

* Queue-based RTL CSV parser: matches `send_packet` events to the earliest unmatched `dispatch_slot_*` or `auth_fallback` event to compute per-request latencies.
* Produces `sw/logs/per_request_latencies.csv` and `sw/logs/rtl_metrics_summary.json`.

### `sw/tests/compare_with_sim.py` and plotting utilities

* Loads latest SIM outputs from `evaluation/logs`, loads RTL per-request CSV, adjusts RTL internal latencies with NIC+PCIe means (from `sim/model_params.py`), and produces comparison plots (`sw/logs/distribution_plot.png`, `comparison_bars.png`) and a `sw/logs/comparison_report.txt`.
* Key outputs: distribution CDFs, histograms, summary report comparing SIM vs RTL-adjusted metrics.

### `sim/run_experiment.py`, `simulation_engine.py`, `model_params.py`

* Event-driven Python simulator and experiment runner. `model_params.py` exposes NIC/PCIe and HDU cycle latencies used to construct end-to-end timing. `run_experiment.py` runs the configured workload (default: 10k requests) and writes `evaluation/logs/sim_results_{TS}_*.npy` plus a summary JSON with the stats used in the README.
* `sweep_missrate.py` runs experiments across miss-rate values to generate `speedup_vs_missrate.png`.


---

## Simulation & analysis pipeline (method)

* **SIM**: parameterized event-driven model in `sim/` (parameters in `sim/model_params.py`). Produces `sim_results_{TS}_*.npy` and summary JSONs for statistical analysis.
* **RTL**: Verilator harness emits `evaluation/logs/rtl_run_*.csv` with event timelines. `sw/tests/parse_rtl_logs.py` pairs sends with responses and outputs per-request latencies.
* **Comparison**: `sw/tests/compare_with_sim.py` aligns RTL internal latencies with NIC/PCIe means from `sim/model_params.py` to create an approximate end-to-end RTL distribution.

All scripts write outputs into `sw/logs` and `evaluation/plots` for convenience.

---

## Results & figures included

### Representative numeric results (from included run files)

**SIM (10k requests)**

```
Baseline mean: 50.59758 µs
Baseline P99: 72.68722 µs
FastPath mean: 6.82610 µs
FastPath P99: 60.93812 µs
Speedup (mean): 7.41238x
```

**RTL micro-bench (Verilator small TB)**

```
total_sends: 9
successes: 1
failures: 1
timeouts: 7
internal dispatch mean: 0.02 µs
RTL-adjusted mean (nic+pcie + internal): 4.02 µs
```

**Synthesis (Yosys flattened `hdu_top`)**

```
Number of wires:    441
Number of wire bits:5436
Number of cells:    425
  DFFs (registers): 32
  MUX cells:        279
  Comparators:      78
```

### Plots included (path)

* `evaluation/plots/cdf_baseline_fastpath.png`
* `evaluation/plots/histogram_baseline_fastpath.png`
* `evaluation/plots/hist_miss_vs_hit.png`
* `evaluation/plots/threshold_fractions.png`
* `evaluation/plots/speedup_vs_missrate.png`
* `sw/logs/distribution_plot.png`
* `sw/logs/comparison_bars.png`

---

## Synthesis & how to reproduce area metrics

1. Ensure `synthesis/rtl` contains the Yosys-friendly RTL (includes `hdu_defs.svh`).
2. Run `yosys -l outputs/hdu_synth.log -s hdu_synth.ys` in `synthesis/`.
3. Inspect `synthesis/outputs/hdu_synth.log` and find the **flattened** `=== hdu_top ===` block — this provides the `Number of cells` and register counts used in this README.

If you want device-specific LUT/FF/Fmax numbers, run `nextpnr` for an FPGA target (ECP5 recommended) using the produced JSON/netlist.

---

## Files with Makefiles and commands

This repo includes Makefile helpers where relevant. Use them as short aliases for the commands above.

* `hw/Makefile` — builds/runs Verilator harness (`make run`).
* `sw/Makefile` — (if present) may contain helpers such as `make plots` or `make compare` — otherwise run Python scripts directly.

---

## Supervision & authorship

* **Kush Kapoor** — Research implementer
* **Supervisor:** Prof. Dr. Jasraj Meena (DTU)

---

## Limitations & future work

* Increase RTL TB stimulus (import SIM traces) to collect robust RTL statistics.
* Replace CAM register array with block RAM primitives for larger tables to save LUTs.
* Run device P&R for Fmax and concrete LUT/FF counts.
* Add power/energy estimation using gate-level toggles.

---

## License & contact

MIT License

**Contact:** [kushkapoor.kk1234@gmail.com](mailto:kushkapoor.kk1234@gmail.com)

---
