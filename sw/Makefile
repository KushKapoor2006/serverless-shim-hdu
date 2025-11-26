# sw/Makefile
PY = python3
TEST_DIR = tests
LOG_DIR = logs

.PHONY: all parse compare run

all: run

parse:
	@echo "Parsing RTL logs..."
	@cd $(TEST_DIR) && $(PY) parse_rtl_logs.py

compare:
	@echo "Comparing with sim..."
	@cd $(TEST_DIR) && $(PY) compare_with_sim.py

run:
	@cd $(TEST_DIR) && ./run_all.sh

clean:
	rm -rf $(LOG_DIR)/*
