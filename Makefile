SHELL := /bin/bash
BIN_DIR := bin

BUILD_TYPE ?= Debug
BUILD_RUST ?= 0
YEAR ?= 2023

RUST_SOURCES = $(wildcard src/$(YEAR)/Day*/solve.rs)
RUST_EXES = $(patsubst src/$(YEAR)/%/solve.rs, $(BIN_DIR)/rust/$(YEAR)_%, $(RUST_SOURCES))

CMAKE_EXPORT_COMPILE_COMMANDS := -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE
CMAKE_BUILD_TYPE_FLAG := -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
CMAKE_CXX_COMPILER_FLAG := -DCMAKE_CXX_COMPILER:STRING=clang++
CMAKE_C_COMPILER_FLAG := -DCMAKE_C_COMPILER:STRING=clang

CMAKE_FLAGS := $(CMAKE_EXPORT_COMPILE_COMMANDS) $(CMAKE_BUILD_TYPE_FLAG) $(CMAKE_CXX_COMPILER_FLAG) $(CMAKE_C_COMPILER_FLAG)

.PHONY: all build init add clean help

help:
	@echo "Usage: make [all|init|add|clean] [YEAR=XXXX] [DAY=XX]"
	@echo "  all: compile all solutions"
	@echo "  init: create bin directory"
	@echo "  add: create new day directory and copy template"
	@echo "  clean: remove all compiled binaries"

all: init config build
	
config:
	cmake ${CMAKE_FLAGS} -S . -B build

build: $(if $(filter 1,$(BUILD_RUST)),$(RUST_EXES))
	cmake --build build

init:
	mkdir -p $(BIN_DIR)/{cpp,rust}

add:
	mkdir -p src/$(YEAR)/Day$(DAY)
	touch src/$(YEAR)/Day$(DAY)/{input,example}.txt
	cp template/solve.cpp src/$(YEAR)/Day$(DAY)/solve.cpp

$(BIN_DIR)/rust/$(YEAR)_%: src/$(YEAR)/%/solve.rs
	rustc $< -o $@

clean:
	rm -rf $(BIN_DIR)
