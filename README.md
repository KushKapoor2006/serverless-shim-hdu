# Serverless HDU — README

---

## TL;DR

This repository implements a hardware **HDU (Hardware Dispatch Unit)** for serverless-style request dispatching. It includes:

* Cycle-accurate **simulations** (Python + SIM model) of baseline and fast-path request processing,
* A **SystemVerilog RTL** implementation of the HDU (auth CAM, slot allocator, fallback FIFO),
* An **open-source synthesis** flow using **Yosys** (produces flattened/hierarchical netlists and stats),
* Scripts to parse RTL run logs and compare RTL results with the SIM model, and
* Plotting scripts that produce the figures included in `sw/logs` and `/evaluation/plots`.

---

## Highlights & quick metrics

* **End-to-end simulation:** Baseline mean latency **~50.60 µs**, FastPath mean latency **~6.83 µs**; **mean speedup ≈ 7.41×** (SIM `sim_results.json`, 10k requests).  FastPath P99 ≈ **60.94 µs**; Baseline P99 ≈ **72.69 µs**.
* **RTL (cycle-accurate) micro-bench:** 9 sends (test harness); RTL recorded **1 dispatch** and **1 fallback** (internal dispatch mean = **0.02 µs**). Adjusting RTL internal latency with NIC+PCIe model gives **RTL-adjusted full-path mean ≈ 4.02 µs**, implying **SIM/RTL speedup ≈ 1.57×** for fast-path in this run.
* **Synthesis (Yosys, flattened)** for `hdu_top` (generic mapping): **Number of cells: 425**, **DFFs (reg count): 32**, **Mux cells: 279**; **Number of wires: 441** (5,436 wire bits). These are the flattened, technology-neutral figures, can be used for area/effort.

> Note: the SIM numbers come from `evaluation/logs/sim_results_*.json` , the RTL numbers come from `sw/logs/rtl_metrics_summary.json` and the Yosys outputs are in `synthesis/outputs`.

---

## Repo file structure (top-level)

```
serverless-shim-hdu/
├─ evaluation/                  # simulator outputs & RTL run logs & plots
│  ├─ logs/
│  └─ plots/
|
├─ hw/
│  ├─ obj_dir/
│  ├─ rtl/                      # SystemVerilog RTL source files
|  ├─ Makefile                  # top-level build / run helpers (verilator / sim)
|  ├─ verilator_harness.cpp     # Verilator harness
|
├─ sim/                         # Python-based SIM model + experiment scripts
|
├─ sw/                          # host-side helper scripts, log parsing + comparison
│  ├─ tests/                    # parse_rtl_logs.py, compare_with_sim.py
│  └─ logs/                     # Software test logs and plots
|
├─ synthesis/                   # synthesis artifacts and Yosys scripts
│  ├─ outputs/                  # writeouts from Yosys (json, verilog, logs)
│  └─ hdu_synth.ys              # Yosys script
|
├─ Serverless - XPU Shim.pdf    # Orignal Reference paper for Serverless Computing on Heterogeneous Computers
└─ README.md
```

---

## Getting started — Dependencies

Install the following (minimum recommended):

* **Python 3.8+** with `numpy`, `matplotlib` (for plotting), and `pandas` (optional).
* **Verilator** (for RTL simulation / TB runs).
* **Yosys** (for open-source synthesis).
* **Graphviz** (optional — convert `.dot` files to PNG/PDF).

On Ubuntu/Debian, an example install sequence:

```bash
sudo apt update
sudo apt install -y verilator yosys graphviz python3-pip
pip3 install numpy matplotlib pandas
```

> If you plan to do FPGA P&R, install `nextpnr` and the vendor support packages for your target (e.g., ECP5, iCE40). That is optional for the core results above.

---

## Reproducible experiments — step-by-step

Below are the high-level steps to reproduce the main artifacts (simulation results, RTL log parsing, comparison plots and synthesis).

### 1) Run RTL testbench (Verilator)

Build and run the Verilator-based testbench. The repo includes a `Makefile` target `run` (or `sim`) that builds the Verilator model and runs the TB which writes `evaluation/logs/rtl_run_*.csv`.

```bash
# from repo root
make run        # builds Verilator model and runs the TB
# or, if you prefer to run Verilator manually, use the project Makefile commands
```

The TB writes CSV logs in `evaluation/logs` (example: `rtl_run_YYYYMMDDTHHMMSS.csv`). Example RTL log lines:

```
event,cycle,ns
reset_done,60,300.0
cfg_write_0,72,360.0
send_packet,74,370.0
dispatch_slot_0,78,390.0
... (truncated)
```

### 2) Parse RTL logs into per-request metrics

Use the included parser to convert RTL CSV timelines into per-request latencies and a summary JSON.

```bash
python3 sw/tests/parse_rtl_logs.py
```

Outputs: `sw/logs/per_request_latencies.csv` and `sw/logs/rtl_metrics_summary.json`.

Example summary (from the run in this repository):

```json
{
  "total_sends": 9,
  "successes": 1,
  "failures": 1,
  "timeouts": 7,
  "success_rate": 0.1111111111111111,
  "dispatch_stats": { "count": 1, "mean_us": 0.02, "p99_us": 0.02 },
  "fallback_stats": { "count": 1, "mean_us": 0.02 }
}
```

> The sample TB run here is intentionally small (few sends) so per-run counts are low — the sim model uses 10k requests for statistical analysis.

### 3) Run simulation experiments (SIM model)

The SIM model in `sim/` generates large-scale request traces and produces `sim_results_{TS}_*.npy` and `sim_results_{TS}_summary.json` files in `evaluation/logs`.

```bash
# run the SIM experiment (example script name; adjust as necessary)
python3 sim/run_experiment.py
# or run a sweep of miss rates
python3 sim/sweep_missrate.py
```

The produced `sim_results_*.json` contains the simulation statistics (10k requests were used in the run included here). Example `stats` snippet (from `sim_results_20251124...`) — key numbers:

```json
{
  "Baseline Mean": 50.59758,
  "Baseline P99": 72.68722,
  "FastPath Mean": 6.82610,
  "FastPath P99": 60.93812,
  "Speedup (Mean)": 7.41238
}
```

### 4) Compare RTL vs SIM and produce plots

The helper script reads the latest simulation outputs in `evaluation/logs`, reads the RTL per-request CSV (`sw/logs/per_request_latencies.csv`) and produces comparison plots and a short text report.

```bash
python3 sw/tests/compare_with_sim.py
```

Outputs placed in `sw/logs/` and `evaluation/plots/` include figures such as:

* `cdf_baseline_fastpath.png`
* `hist_miss_vs_hit.png`
* `distribution_plot.png`
* `comparison_bars.png`
* `speedup_vs_missrate.png`

(Pre-generated images for the included run are in `evaluation/plots`.)

### 5) Synthesis (Yosys)

A Yosys script (`synthesis/hdu_synth.ys`) produces hierarchical and flattened netlists and statistics. To run the synthesis flow:

```bash
cd synthesis
# run Yosys and log output
yosys -l outputs/hdu_synth.log -s hdu_synth.ys
```

This writes to `synthesis/outputs/`:

* `hdu_top_hier.v`, `hdu_top_hier.json` — hierarchical netlist and JSON
* `hdu_top_flat.v`, `hdu_top_flat.json` — flattened netlist and JSON
* `hdu_synth.log` — Yosys log with `stat` sections
* `hdu_top_flat.dot` (optional): DOT schematic of flattened netlist

The flattened `stat` block (from the included run) contains:

```
=== hdu_top ===
   Number of wires:                441
   Number of wire bits:           5436
   Number of cells:                425
     $add                            1
     $adff                           7
     $and                            3
     $dff                           32
     $eq                            78
     $mux                          279
     ...
```

These numbers are technology-neutral and useful for resume/area estimation. If you want device-specific area/Fmax, run `nextpnr` (target e.g. Lattice ECP5) on `hdu_top_flat.json`.

---

## Figures & logs included (examples in repo)

The repository includes example plots and logs (from the simulation and RTL comparison):

* `evaluation/plots/cdf_baseline_fastpath.png` (CDFs of latency)
* `evaluation/plots/hist_miss_vs_hit.png` (histograms of hits vs misses)
* `evaluation/plots/distribution_plot.png` (combined distributions)
* `synthesis/outputs/hdu_synth.log` (Yosys output & stats)
* `sw/logs/rtl_metrics_summary.json` (RTL per-request summary)
* `evaluation/logs/rtl_run_*.csv` (raw RTL event timeline CSV)

Use these in figures for a report or application. They are reproducible via the commands above.

---

## How results were computed (methodology)

* **SIM model**: a Python-based event-driven model (in `sim/`) synthesizes request paths through NIC/PCIe/host stack + HDU micro-steps. Model parameters (NIC, PCIe, HDU cycle counts) are in `sim/model_params.py`. Results are statistical (10k requests by default) and reported in microseconds.

* **RTL runs**: the Verilator testbench emits timestamped `event,cycle,ns` records. `sw/tests/parse_rtl_logs.py` uses a queue-based pairing strategy to match `send_packet` events to `dispatch_slot_*` or `auth_fallback` responses and computes per-request latencies (cycles → µs via clock period).

* **RTL-to-SIM comparison**: `sw/tests/compare_with_sim.py` imports the SIM summary and adds NIC+PCIe mean latencies to the RTL internal dispatch latency to form an approximate end-to-end RTL-adjusted latency distribution. This is how the `RTL adjusted mean ≈ 4.02 µs` figure was derived.

* **Synthesis**: Yosys `synth` → `flatten` gives a technology-agnostic gate-level view. The `Number of cells` reported is a generic gate-cell count under Yosys’s internal cell model.

---

## Reproducibility notes & recommended next steps

* The included TB run for RTL is small (designed to demonstrate the pipeline); to collect more comprehensive RTL stats, expand the TB stimulus or convert a SIM    trace into a TB driver.
* For technology-specific metrics (LUTs/FFs/Fmax/power) target an FPGA and run `nextpnr` + vendor bitstream tools (ECP5 recommended for open-source flow).
* For power estimation, use a VCD from a gate-level simulation and a power estimation tool (vendor or open-source approximators).

---

## Development notes / design trade-offs

* I replaced SystemVerilog `package` usage with a `hdu_defs.svh` `include` to guarantee compatibility with Yosys’ SystemVerilog frontend and to produce clean synthesis outputs.
* The auth CAM is a small CAM table implemented using registers (no RAM primitives inferred in this run); the fallback bridge is a small software-facing FIFO.
* The slot allocator uses a bitmap-first-free allocation strategy to minimize logic depth and control state.

---

## Contact / Attribution

Author: Kush Kapoor (GitHub: `KushKapoor2006`)

