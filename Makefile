SHELL := /bin/bash
BIN_DIR := bin

BUILD_TYPE ?= Debug
YEAR ?= 2023

RUST_SOURCES = $(wildcard src/$(YEAR)/Day*/solve.rs)
RUST_EXES = $(patsubst src/$(YEAR)/%/solve.rs, $(BIN_DIR)/rust/$(YEAR)_%, $(RUST_SOURCES))

CMAKE_FLAGS=-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=${BUILD_TYPE}
CMAKE_COMPILER_FLAGS=-DCMAKE_CXX_COMPILER:STRING=clang++ -DCMAKE_C_COMPILER:STRING=clang

.PHONY: all build init add clean help

help:
	@echo "Usage: make [all|init|add|clean] [YEAR=XXXX] [DAY=XX]"
	@echo "  all: compile all solutions"
	@echo "  init: create bin directory"
	@echo "  add: create new day directory and copy template"
	@echo "  clean: remove all compiled binaries"

all: init config build $(RUST_EXES)
	
config:
	cmake ${CMAKE_FLAGS} ${CMAKE_COMPILER_FLAGS} -S . -B build

build:
	cmake --build build

init:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BIN_DIR)/cpp
	mkdir -p $(BIN_DIR)/rust

add:
	mkdir -p src/$(YEAR)/Day$(DAY)
	touch src/$(YEAR)/Day$(DAY)/{input,example}.txt
	cp template/solve.cpp src/$(YEAR)/Day$(DAY)/solve.cpp

$(BIN_DIR)/rust/$(YEAR)_%: src/$(YEAR)/%/solve.rs
	rustc $< -o $@

clean:
	rm -rf bin
