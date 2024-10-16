SHELL := /bin/bash
BIN_DIR := bin
VCPKG_DIR := $(HOME)/package/vcpkg/installed/x64-linux

CPP_SOURCES = $(wildcard src/$(YEAR)/Day*/solve.cpp)
CPP_EXES = $(patsubst src/%/solve.cpp, $(BIN_DIR)/cpp/%, $(CPP_SOURCES))

RUST_SOURCES = $(wildcard src/$(YEAR)/Day*/solve.rs)
RUST_EXES = $(patsubst src/%/solve.rs, $(BIN_DIR)/rust/%, $(RUST_SOURCES))

CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(VCPKG_DIR)/include -I./include -L$(VCPKG_DIR)/lib -lfmt

.PHONY: all init add clean help

help:
	@echo "Usage: make [all|init|add|clean] [YEAR=XXXX] [DAY=XX]"
	@echo "  all: compile all cpp solutions"
	@echo "  init: create bin directory"
	@echo "  add: create new day directory and copy template"
	@echo "  clean: remove all compiled binaries"

all: init $(CPP_EXES) $(RUST_EXES)

init:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/cpp/$(YEAR)
	mkdir -p $(BIN_DIR)/rust/$(YEAR)

add:
	mkdir -p src/$(YEAR)/Day$(DAY)
	touch src/$(YEAR)/Day$(DAY)/{input,example}.txt
	cp template/solve.cpp src/$(YEAR)/Day$(DAY)/solve.cpp

$(BIN_DIR)/cpp/%: src/%/solve.cpp
	$(CXX) $< $(CXXFLAGS) -o $@

$(BIN_DIR)/rust/%: src/%/solve.rs
	rustc $< -o $@

clean:
	rm -f $(CPP_EXES)
