# Advent of Code

Personal solutions to [Advent of Code](https://adventofcode.com) puzzles in C++ and Rust.

## Usage

```bash
# Create and run (auto-detects current year)
make add DAY=5
make run DAY=5

# Rust
make add DAY=5 LANGUAGE=rust
make run DAY=5 LANGUAGE=rust

# Regenerate rust-project.json for IDE support
make rust-project
```

## Structure

```
src/YYYY/DayXX/
  solve.cpp       # C++ solution
  solve.rs        # Rust solution (optional)
  input.txt
  example.txt
```
