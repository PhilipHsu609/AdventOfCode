use std::fs;
use std::path::Path;
use std::time::Instant;
use std::{env, file};

fn set_working_dir() {
    let path = Path::new(file!());
    env::set_current_dir(path.parent().unwrap()).unwrap();
}

fn part1(input: &str) -> i64 {
    0
}

fn part2(input: &str) -> i64 {
    0
}

fn main() {
    set_working_dir();

    let example = fs::read_to_string("example.txt").expect("Failed to read example.txt");
    let input = fs::read_to_string("input.txt").expect("Failed to read input.txt");

    let part1_example_ans = 0;
    let part2_example_ans = 0;

    assert_eq!(part1(&example), part1_example_ans, "Part1 example failed");
    assert_eq!(part2(&example), part2_example_ans, "Part2 example failed");

    let t1 = Instant::now();
    let p1 = part1(&input);
    let t2 = Instant::now();
    let p2 = part2(&input);
    let t3 = Instant::now();

    println!("Part1: {} ({:.3?})", p1, t2 - t1);
    println!("Part2: {} ({:.3?})", p2, t3 - t2);
}
