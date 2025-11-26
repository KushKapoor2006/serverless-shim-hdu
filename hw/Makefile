# hw/Makefile
VERILATOR = verilator
VERILATOR_FLAGS = --cc --exe -Wno-fatal --top-module hdu_top --build

RTL_DIR = rtl
SV_SRCS = $(RTL_DIR)/hdu_pkg.sv \
          $(RTL_DIR)/header_parser.sv \
          $(RTL_DIR)/auth_cam.sv \
          $(RTL_DIR)/slot_allocator.sv \
          $(RTL_DIR)/fallback_bridge.sv \
          $(RTL_DIR)/hdu_top.sv

CPP_HARNESS = verilator_harness.cpp

OBJ_DIR = obj_dir
BUILD_EXE = Vhdu_top

.PHONY: all clean run build

all: build

build:
	@echo "[make] Building Verilator model..."
	mkdir -p $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_FLAGS) $(SV_SRCS) $(CPP_HARNESS) -I$(RTL_DIR) -Mdir $(OBJ_DIR) -CFLAGS "-std=c++11" -LDFLAGS "-pthread"
	@echo "[make] build complete"

run: build
	@echo "Running RTL sim (Verilator harness)..."
	mkdir -p ../evaluation/logs
	if [ -x $(OBJ_DIR)/$(BUILD_EXE) ]; then \
	  $(OBJ_DIR)/$(BUILD_EXE) +vcd; \
	else \
	  echo "[make] ERROR: expected binary '$(OBJ_DIR)/$(BUILD_EXE)' not found or not executable"; exit 1; \
	fi
	@echo "Simulation finished."

clean:
	rm -rf $(OBJ_DIR) *.o *.vcd ../evaluation/logs/*.csv ../evaluation/logs/*.vcd
