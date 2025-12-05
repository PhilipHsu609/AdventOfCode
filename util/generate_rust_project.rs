#!/usr/bin/env rust-script
//! Generate rust-project.json for rust-analyzer support
//!
//! Usage: rustc generate_rust_project.rs && ./generate_rust_project

use std::env;
use std::fs;
use std::path::{Path, PathBuf};
use std::process::Command;

#[derive(Debug)]
struct RustCrate {
    display_name: String,
    root_module: String,
    edition: String,
}

fn find_sysroot_src() -> Option<String> {
    // Try to get sysroot from rustc
    let output = Command::new("rustc")
        .arg("--print")
        .arg("sysroot")
        .output()
        .ok()?;

    let sysroot = String::from_utf8(output.stdout).ok()?;
    let sysroot = sysroot.trim();

    // Standard location within sysroot
    let src_path = PathBuf::from(sysroot)
        .join("lib/rustlib/src/rust/library");

    if src_path.exists() {
        Some(src_path.to_string_lossy().to_string())
    } else {
        None
    }
}

fn find_rust_crates(src_dir: &Path) -> Vec<RustCrate> {
    let mut crates = Vec::new();

    // Walk through src directory structure: src/YYYY/DayXX/solve.rs
    if let Ok(years) = fs::read_dir(src_dir) {
        for year_entry in years.flatten() {
            let year_path = year_entry.path();
            if !year_path.is_dir() {
                continue;
            }

            let year_name = year_entry.file_name().to_string_lossy().to_string();

            // Look for Day directories
            if let Ok(days) = fs::read_dir(&year_path) {
                for day_entry in days.flatten() {
                    let day_path = day_entry.path();
                    if !day_path.is_dir() {
                        continue;
                    }

                    let day_name = day_entry.file_name().to_string_lossy().to_string();
                    let solve_rs = day_path.join("solve.rs");

                    if solve_rs.exists() {
                        let root_module = format!("src/{}/{}/solve.rs", year_name, day_name);
                        let display_name = format!("{} {}", year_name, day_name);

                        crates.push(RustCrate {
                            display_name,
                            root_module,
                            edition: "2021".to_string(),
                        });
                    }
                }
            }
        }
    }

    // Sort by year and day for consistency
    crates.sort_by(|a, b| a.root_module.cmp(&b.root_module));

    crates
}

fn generate_json(crates: &[RustCrate], sysroot_src: Option<String>) -> String {
    let mut json = String::from("{\n");

    // Add sysroot_src if found
    if let Some(sysroot) = sysroot_src {
        json.push_str(&format!("  \"sysroot_src\": \"{}\",\n", sysroot));
    }

    // Add crates array
    json.push_str("  \"crates\": [\n");

    for (i, krate) in crates.iter().enumerate() {
        json.push_str("    {\n");
        json.push_str(&format!("      \"display_name\": \"{}\",\n", krate.display_name));
        json.push_str(&format!("      \"root_module\": \"{}\",\n", krate.root_module));
        json.push_str(&format!("      \"edition\": \"{}\",\n", krate.edition));
        json.push_str("      \"deps\": []\n");
        json.push_str("    }");

        if i < crates.len() - 1 {
            json.push_str(",\n");
        } else {
            json.push('\n');
        }
    }

    json.push_str("  ]\n");
    json.push_str("}\n");

    json
}

fn main() {
    let current_dir = env::current_dir()
        .expect("Failed to get current directory");

    let src_dir = current_dir.join("src");
    if !src_dir.exists() {
        eprintln!("Error: src directory not found");
        eprintln!("Run this from the project root");
        std::process::exit(1);
    }

    println!("Scanning for Rust solutions...");
    let crates = find_rust_crates(&src_dir);

    if crates.is_empty() {
        println!("No Rust solutions found (looking for src/YYYY/DayXX/solve.rs)");
        std::process::exit(0);
    }

    println!("Found {} Rust crate(s)", crates.len());
    for krate in &crates {
        println!("  - {}", krate.display_name);
    }

    println!("\nDetecting sysroot...");
    let sysroot_src = find_sysroot_src();
    match &sysroot_src {
        Some(path) => println!("  Found: {}", path),
        None => println!("  Not found (rust-analyzer will use default)"),
    }

    let json = generate_json(&crates, sysroot_src);

    let output_path = current_dir.join("rust-project.json");
    fs::write(&output_path, json)
        .expect("Failed to write rust-project.json");

    println!("\n✓ Generated rust-project.json");
    println!("  Reload your editor to apply changes");
}
