use std::path::Path;
use std::{env, file, fs};

fn part1() {}

fn part2() {}

fn main() {
    let path = Path::new(file!());
    env::set_current_dir(&path.parent().unwrap()).unwrap();

    part1();
    part2();
}
