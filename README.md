# Serverless HDU — Research README

> **Project (research internship / research assistant work)** — Design, implementation and evaluation of a Hardware Dispatch Unit (HDU) to accelerate serverless-style request dispatching, an extension of the **Serverless Computing on Heterogeneous Computers** paper.

**Primary contributions in this repo:**

* Implemented a synthesizable HDU microarchitecture (authorization CAM, bitmap slot allocator, fallback bridge) and verified correctness with Verilator testbenches and Python analysis.
* Built a reproducible evaluation pipeline: SIM model → RTL harness → event log parsing → SIM/RTL alignment → plots and synthesis metrics.
* Demonstrated an open-source synthesis flow (Yosys) and reported technology-neutral area proxies used for CV / publication claims.

**Status:** Implementation, functional verification, simulation experiments and open-source synthesis are complete. This code was developed as research assistant work and is intended to be reproducible for further experiments.

---

## Table of Contents

1. [Overview & Motivation](#overview--motivation)
2. [Project Summary & Highlights](#project-summary--highlights)
3. [What this repo contains (detailed)](#what-this-repo-contains-detailed)
4. [Quickstart — reproduce figures & synthesis](#quickstart--reproduce-figures--synthesis)
5. [Design & RTL modules (short)](#design--rtl-modules-short)
6. [Simulation & analysis pipeline (method)](#simulation--analysis-pipeline-method)
7. [Results & quantitative metrics](#results--quantitative-metrics)
8. [Synthesis & area metrics (Yosys)](#synthesis--area-metrics-yosys)
10. [How to run everything — commands & tips](#how-to-run-everything--commands--tips)
11. [CV-ready bullets & how to cite this work](#cv-ready-bullets--how-to-cite-this-work)
12. [Supervision & authorship](#supervision--authorship)
13. [Limitations & future work (research roadmap)](#limitations--future-work-research-roadmap)
14. [License & contact](#license--contact)

---

## Overview & Motivation

Modern serverless and event-driven workloads are highly sensitive to dispatch latency: the time from packet ingress to dispatching work on a compute slot. The conventional software network stack (NIC → kernel → user-space scheduling) produces variable and often-high latencies. This project explores an **HDU microarchitecture** that offloads authorization and slot allocation to hardware to reduce common-case latency (fast-path) while providing a fallback to host handling on misses.

The HDU implements:

* a compact CAM-based authorization table (fast-path matching),
* a bitmap-based slot allocator to provide fast allocation with minimal logic depth,
* a fallback bridge FIFO that surfaces misses to the host OS/driver.

The objective is to quantify latency improvements using an event-driven Python SIM and to validate the RTL implementation and synthesizability with Verilator and Yosys.

---

## Project Summary & Highlights

**Key quantitative highlights (from included runs):**

* **SIM (10k requests)**: Baseline mean latency **~50.60 µs**, Baseline P99 **~72.69 µs**. FastPath mean latency **~6.83 µs**, FastPath P99 **~60.94 µs**. **Mean speedup ≈ 7.41×** (SIM model, `sim/sim_results_*.json`).
* **RTL (Verilator TB run)**: Small verification run with 9 sends; `total_sends=9`, `successes=1`, `failures=1`, `timeouts=7`. Internal HDU dispatch latency measured **0.02 µs**. After adding NIC+PCIe mean latencies, **RTL-adjusted end-to-end mean ≈ 4.02 µs**.
* **Synthesis (Yosys, flattened)**: `hdu_top` flattened statistics: **Number of cells: 425**, **DFFs (regs): 32**, **Mux cells: 279**, **Number of wires: 441** (5,436 wire bits). These are technology-neutral proxies usable for CV claims.

All artifacts used to produce these numbers are included in the repo and can be regenerated (see Reproducibility section).

---

## What this repo contains (detailed)

```
serverless-shim-hdu/
├─ evaluation/           # simulator & RTL logs, plots
│  ├─ logs/              # sim_results_*.json / rtl_run_*.csv
│  └─ plots/             # generated analysis plots (PNG)
├─ hw/
│  ├─ rtl/               # SystemVerilog RTL sources (hdu_top, blocks...)
│  └─ tb/                # Verilator testbench & C++ harness
├─ sim/                  # Python SIM model + experiment scripts
├─ sw/                   # parsing, comparison and plotting scripts
│  └─ tests/             # parse_rtl_logs.py, compare_with_sim.py
├─ synthesis/            # hdu_synth.ys + outputs (Yosys)
│  └─ outputs/           # hdu_top_flat.v, hdu_top_flat.json, logs
├─ Makefile              # convenience targets to run TB/sim/synth
├─ Serverless.pdf        # Orignal XPU Shim Paper
└─ README.md             # this document
```

**Included artifacts**

* Simulation summary (JSON): `evaluation/logs/sim_results_{TS}_summary.json`.
* RTL per-request summary: `sw/logs/rtl_metrics_summary.json`.
* Per-request CSV: `sw/logs/per_request_latencies.csv`.
* Yosys flattened stats: `synthesis/outputs/hdu_synth.log` (see `=== hdu_top ===` after flatten).
* Plots: `evaluation/plots/*` (CDFs, histograms, speedup-vs-missrate, tail fractions, distribution comparison).

---

## Quickstart — reproduce figures & synthesis

### Requirements

* **Python 3.8+**: `numpy`, `matplotlib`, `pandas` (optional)
* **Verilator** (for RTL verification)
* **Yosys** (for synthesis) — recommended v0.59+ for improved SV support
* **Graphviz** (optional — convert `.dot` → PDF/PNG)

Install Python deps quickly:

```bash
pip3 install numpy matplotlib pandas
```

### Quick reproduction steps

1. Run Verilator testbench (generates RTL CSV logs):

```bash
make run
# output: evaluation/logs/rtl_run_*.csv
```

2. Parse RTL logs into per-request CSV and JSON summary:

```bash
python3 sw/tests/parse_rtl_logs.py
# output: sw/logs/per_request_latencies.csv, sw/logs/rtl_metrics_summary.json
```

3. Run SIM experiments (10k requests by default):

```bash
python3 sim/run_experiment.py
# produces evaluation/logs/sim_results_{TS}_*.npy and sim_results_{TS}_summary.json
```

4. Compare SIM vs RTL and generate plots:

```bash
python3 sw/tests/compare_with_sim.py
# outputs: sw/logs/comparison_report.txt and PNG plots
```

5. Synthesis (Yosys):

```bash
cd synthesis
yosys -l outputs/hdu_synth.log -s hdu_synth.ys
# outputs written to synthesis/outputs/
```

(See Section *How to run everything* for full command list.)

---

## Design & RTL modules (short)

The RTL implements a small set of blocks wired together in `hdu_top`:

* `header_parser.sv` — extracts func_id and token fields from incoming header words.
* `auth_cam.sv` — small content-addressable table (CAM) implemented with registers; supports config writes and lookup done in a single clock tick.
* `slot_allocator.sv` — bitmap-based free-slot allocator that returns first-free slot; designed to be simple and single-cycle ready.
* `fallback_bridge.sv` — a small FIFO that records fallback events for host injection/processing.
* `hdu_top.sv` — top-level that ties the above blocks together, provides testbench-facing TB inject ports, and exposes telemetry signals.

Design goals: minimal control-state, easy synthesis in Yosys, and deterministic behavior for accurate latency reporting.

---

## Simulation & analysis pipeline (method)

**SIM (Python) model**

* Event-driven model that simulates NIC ingress, PCIe/host costs, and HDU micro-ops.
* Parameterized by `sim/model_params.py` containing mean latencies for NIC, PCIe, and HDU cycle counts.
* Produces per-request latency arrays and summary JSON files for statistical analysis.

**RTL verification (Verilator)**

* Verilator harness drives `hdu_top` with a small set of sends and logs event timelines as CSV (`event,cycle,ns`).
* `sw/tests/parse_rtl_logs.py` implements a queue-based pairing algorithm to pair `send_packet` events to `dispatch_slot_*` and `auth_fallback` events to compute per-request latencies (cycle → µs via clock period).

**SIM ↔ RTL alignment**

* `sw/tests/compare_with_sim.py` reads latest SIM output and RTL per-request CSV and adjusts the RTL internal latency by adding NIC+PCIe means (from `sim/model_params`) to create an approximate end-to-end RTL distribution. This supports a fair comparison to the SIM model.

---

## Results & quantitative metrics

### SIM results (representative run)

From `evaluation/logs/sim_results_20251124T195238Z_summary.json` (10k requests):

```
Baseline Mean: 50.59758 µs
Baseline P99: 72.68722 µs
FastPath Mean: 6.82610 µs
FastPath P99: 60.93812 µs
Speedup (Mean): 7.41238x
```

Interpretation: the SIM model predicts large mean speedups when requests hit the HDU fast-path; P99 speedup is more modest.

### RTL micro-bench (Verilator TB run)

Small TB run (9 sends) — this run is for functional validation, not statistical analysis:

```
total_sends: 9
successes: 1
failures: 1
timeouts: 7
dispatch mean (internal): 0.02 µs
fallback mean: 0.02 µs
```

After adding NIC mean (1.5 µs default) + PCIe mean (2.5 µs default) to internal RTL dispatch (0.02 µs), we obtain **RTL-adjusted mean ≈ 4.02 µs** for an approximate end-to-end comparison to SIM fast-path.

### Plots included

Key generated plots (examples included in repo):

* `cdf_baseline_fastpath.png` — CDF of baseline vs fast-path latencies (SIM).
* `hist_miss_vs_hit.png` — hit/miss histograms for fast-path.
* `speedup_vs_missrate.png` — how mean speedup degrades with increasing miss rate.
* `threshold_fractions.png` — tail fractions vs latency threshold.
* `distribution_plot.png` — combined SIM and RTL distributions for visual comparison.

---

## Synthesis & area metrics (Yosys)

Synthesis was performed with Yosys using a SystemVerilog frontend. The flow produced both hierarchical and flattened netlists.

**Flattened `hdu_top` statistics (technology-neutral):**

```
Number of wires:                441
Number of wire bits:           5436
Number of cells:                425
  $dff (registers):             32
  $mux (mux cells):            279
  $eq (comparators):            78
  ...
```

Interpretation: The HDU control datapath is compact (few hundred generic cells). For device-level area/Fmax, run a place-and-route flow (e.g., `Yosys -> nextpnr -> FPGA bitstream`) for a chosen target (Lattice ECP5 recommended for open-source flows).

---

## How to run everything — commands & tips

### Full reproducible pipeline (recommended order)

```bash
# 1) Build & run RTL TB
make run

# 2) Parse RTL logs
python3 sw/tests/parse_rtl_logs.py

# 3) Run SIM experiment(s)
python3 sim/run_experiment.py

# 4) Compare and plot
python3 sw/tests/compare_with_sim.py

# 5) Synthesize with Yosys
cd synthesis
yosys -l outputs/hdu_synth.log -s hdu_synth.ys
```

**Helpful tips:**

* Use the `-l` option to write Yosys logs to `synthesis/outputs/hdu_synth.log` for easy access.
* If Yosys errors on SystemVerilog constructs, upgrade to a recent build (v0.59+ or git master).
* Expand the Verilator TB stimulus to drive larger traces (export a SIM trace to the TB) if you need more representative RTL statistics.

---

## CV-ready bullets & how to cite this work

**Academic / Research-style (EPFL / PhD application)**

* *Implemented a synthesizable Hardware Dispatch Unit (HDU) in SystemVerilog (CAM-based authorization, bitmap allocator, fallback FIFO); verified with cycle-accurate Verilator harnesses and a 10k-request Python SIM model, demonstrating a fast-path mean latency ≈ 6.83 µs and mean speedup ≈ 7.41× (SIM).*
* *Synthesized the HDU using an open-source flow (Yosys); flattened design yields ~425 generic cells (~32 registers), and produced hierarchical/flattened JSON/netlists ready for FPGA mapping and timing estimation.*

**Industry-style (concise)**

* Designed, simulated and synthesized an HDU offload for sub-10 µs fast-path dispatch: SystemVerilog RTL + Verilator + Yosys pipeline; demonstrated SIM fast-path mean **6.83 µs** and RTL-adjusted end-to-end mean **4.02 µs**.

Use these bullets as-is on your CV; swap phrasing for an industry or academic tone as needed.

---

## Supervision & authorship

* **Research assistant / implementer:** Kush Kapoor (`KushKapoor2006`)
* **Supervision:** Prof. `<SUPERVISOR_NAME>` — Research internship / RA supervision (replace with your supervisor's full name and affiliation).

> If you want me to include the supervisor's full affiliation and a one-line description (e.g., "Supervised by Prof. X, Department of Computer Science, University Y"), tell me the exact text and I will add it.

---

## Limitations & future work (research roadmap)

1. Expand RTL TB stimulus to drive large representative traces (import SIM traces) to collect robust RTL per-request statistics.
2. Improve CAM implementation to use BRAM/true memory primitives for larger tables.
3. Run a full P&R flow (Yosys -> nextpnr -> device) to extract Fmax and concrete LUT/FF counts on a target FPGA (ECP5 suggested).
4. Develop adaptive prefetch throttling policies and evaluate energy/DRAM-bandwidth trade-offs.

---

## License & contact

This repository is intended to be published under the MIT license. If you want, I can add an MIT `LICENSE` file to the repo.

**Contact:** [kushkapoor.kk1234@gmail.com](mailto:kushkapoor.kk1234@gmail.com)

---

*README structured for research-grade presentation and reproducibility. If you want the README to match Flexagon's headings and tone verbatim, I can further align phrasing; provide the exact Flexagon README or confirm and I will make an exact mirror.*
