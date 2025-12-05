SHELL := /bin/bash
BIN_DIR := bin

YEAR ?= $(shell date +%Y)
DAY ?= 1
LANGUAGE ?= cpp

# Computed variables
DAY_PADDED := $(shell printf "%02d" $(DAY))
DAY_DIR := src/$(YEAR)/Day$(DAY_PADDED)
TARGET_EXE := $(BIN_DIR)/$(LANGUAGE)/$(YEAR)_Day$(DAY)

# CMake configuration
CMAKE_EXPORT_COMPILE_COMMANDS := -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
CMAKE_CXX_COMPILER_FLAG := -DCMAKE_CXX_COMPILER:STRING=clang++
CMAKE_C_COMPILER_FLAG := -DCMAKE_C_COMPILER:STRING=clang

CMAKE_FLAGS := $(CMAKE_EXPORT_COMPILE_COMMANDS) $(CMAKE_CXX_COMPILER_FLAG) $(CMAKE_C_COMPILER_FLAG)

.PHONY: all build init add run clean help

help:
	@echo "Usage: make [target] [YEAR=XXXX] [DAY=XX] [LANGUAGE=cpp|rust]"
	@echo ""
	@echo "  all   - compile all solutions"
	@echo "  add   - create new day directory and copy template"
	@echo "  run   - build and run specific solution"
	@echo "  clean - remove all compiled binaries"
	@echo ""
	@echo "Examples:"
	@echo "  make add DAY=5              - create Day05 (C++)"
	@echo "  make add DAY=5 LANGUAGE=rust - create Day05 (Rust)"
	@echo "  make run DAY=5              - run Day05 (C++)"
	@echo "  make run DAY=5 LANGUAGE=rust - run Day05 (Rust)"

all: init config build
	
config:
	cmake ${CMAKE_FLAGS} -S . -B build

build:
	cmake --build build

init:
	mkdir -p $(BIN_DIR)/{cpp,rust}

add:
	@mkdir -p $(DAY_DIR)
	@touch $(DAY_DIR)/input.txt $(DAY_DIR)/example.txt
	@if [ "$(LANGUAGE)" = "rust" ]; then \
		cp template/solve.rs $(DAY_DIR)/solve.rs; \
	else \
		cp template/solve.cpp $(DAY_DIR)/solve.cpp; \
	fi
	@echo "Created $(DAY_DIR)"

run: init
	@if [ "$(LANGUAGE)" = "rust" ]; then \
		rustc $(CURDIR)/$(DAY_DIR)/solve.rs -o $(TARGET_EXE); \
	else \
		test -f build/CMakeCache.txt || cmake $(CMAKE_FLAGS) -S . -B build >/dev/null; \
		cmake --build build --target $(YEAR)_Day$(DAY) 2>&1 | grep -v "Entering\|Leaving\|Built target" || true; \
	fi
	@echo "" && cd $(DAY_DIR) && $(CURDIR)/$(TARGET_EXE)

clean:
	rm -rf $(BIN_DIR)
