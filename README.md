# Advent of Code

Personal solutions to [Advent of Code](https://adventofcode.com) puzzles in C++ and Rust.

## Quick Start

```bash
# Create new day (auto-detects current year)
make add DAY=5

# Run solution
make run DAY=5

# Rust support
make add DAY=5 LANGUAGE=rust
make run DAY=5 LANGUAGE=rust

# Build all C++ solutions
make all
```

## Structure

```
src/
  YYYY/
    Day01/
      solve.cpp     # C++ solution
      solve.rs      # Rust solution (optional)
      input.txt     # Puzzle input
      example.txt   # Example input
```
