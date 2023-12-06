use std::fs;
use std::collections::HashMap;

fn part1() {
    let input = fs::read_to_string("input.txt").unwrap();
    let mut sum = 0;
    
    for line in input.lines() {
        let (game, rest) = line.split_once(": ").unwrap();
        let id = game.strip_prefix("Game ").unwrap().parse::<u32>().unwrap();

        let is_valid = rest.split("; ").all(|sample| {
            sample.split(", ").all(|cube| {
                let (count, color) = cube.split_once(" ").unwrap();
                let count = count.parse::<u32>().unwrap();

                match color {
                    "red" => count <= 12,
                    "green" => count <= 13,
                    "blue" => count <= 14,
                    _ => false,
                }
            })
        });

        if is_valid {
            sum += id;
        }
    }

    println!("{}", sum);
}

fn part2() {
    let input = fs::read_to_string("input.txt").unwrap();
    let sum = input.lines().map(|line| {
        let mut sample: HashMap<_, _> = [
            ("red", 0),
            ("green", 0),
            ("blue", 0),
        ].iter().cloned().collect();
        
        let mut iter = line.split_ascii_whitespace().skip(2); // Skip "Game <id>:"
        while let Some(count) = iter.next() {
            let count = count.parse::<u32>().unwrap();
            let color = iter.next().unwrap().trim_end_matches([',', ';']);
            
            let entry = sample.get_mut(color).unwrap();
            if count > *entry {
                *entry = count;
            }
        }

        sample.values().product::<u32>()
    }).sum::<u32>();

    println!("{}", sum);
}

fn main() {
    /*
    References:
        https://www.reddit.com/r/adventofcode/comments/188w447/comment/kbtczzc/?utm_source=share&utm_medium=web2x&context=3
        https://github.com/LinAGKar/advent-of-code-2023-rust/blob/master/day2/src/main.rs
    */
    part1();
    part2();
}