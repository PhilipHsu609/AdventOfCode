use std::env;
use std::fs;
use std::path::Path;
use std::time::Instant;

fn set_working_dir() {
    let path = Path::new(file!());
    env::set_current_dir(path.parent().unwrap()).unwrap();
}

// Custom input parser - modify based on problem
fn read_input(path: &str) -> Vec<String> {
    fs::read_to_string(path)
        .expect(&format!("Failed to read {}", path))
        .lines()
        .map(|s| s.to_string())
        .collect()
}

// Helper functions here

fn part1<T>(input: &[T]) -> i64 {
    0
}

fn part2<T>(input: &[T]) -> i64 {
    0
}

fn main() {
    set_working_dir();

    let example = read_input("example.txt");
    let input = read_input("input.txt");
    let part1_example_ans = 0;
    let part2_example_ans = 0;

    assert_eq!(part1(&example), part1_example_ans, "Part1 example failed");
    assert_eq!(part2(&example), part2_example_ans, "Part2 example failed");

    let t1 = Instant::now();
    let p1 = part1(&input);
    let elapsed1 = t1.elapsed().as_secs_f64() * 1000.0;

    let t2 = Instant::now();
    let p2 = part2(&input);
    let elapsed2 = t2.elapsed().as_secs_f64() * 1000.0;

    println!("Part1: {} ({:.3}ms)", p1, elapsed1);
    println!("Part2: {} ({:.3}ms)", p2, elapsed2);
}
