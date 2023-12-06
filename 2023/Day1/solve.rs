use std::fs;
use std::collections::HashMap;

fn part1() {
    let input = fs::read_to_string("input.txt").unwrap();

    let mut sum = 0;
    for line in input.lines() {
        let first = line.chars().filter(|c| c.is_digit(10)).next().unwrap();
        let last = line.chars().filter(|c| c.is_digit(10)).last().unwrap();

        sum += first.to_digit(10).unwrap() * 10 + last.to_digit(10).unwrap();
    }

    println!("Sum: {}", sum);
}

fn part2() {
    let input = fs::read_to_string("input.txt").unwrap();
    let mut dict = HashMap::new();

    dict.insert(String::from("one"), 1);
    dict.insert(String::from("two"), 2);
    dict.insert(String::from("three"), 3);
    dict.insert(String::from("four"), 4);
    dict.insert(String::from("five"), 5);
    dict.insert(String::from("six"), 6);
    dict.insert(String::from("seven"), 7);
    dict.insert(String::from("eight"), 8);
    dict.insert(String::from("nine"), 9);

    let mut sum = 0;
    for line in input.lines() {
        let mut first = -1;
        let mut last = 0;

        for (i, c) in line.chars().enumerate() {
            if c.is_digit(10) {
                if first == -1 {
                    first = c.to_digit(10).unwrap() as i32;
                }
                last = c.to_digit(10).unwrap() as i32;
            } else {
                for (key, val) in &dict {
                    if line[i..].starts_with(key) {
                        if first == -1 {
                            first = *val;
                        }
                        last = *val;
                    }
                }
            }
        }

        sum += first * 10 + last;
    }

    println!("{}", sum);
}

fn main() {
    part1();
    part2();
}