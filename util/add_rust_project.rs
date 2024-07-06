extern crate serde_json;

use serde_json::{json, Value};
use std::env;
use std::fs::OpenOptions;
use std::io::{self, Read, Seek, Write};

fn main() -> io::Result<()> {
    let args = env::args().collect::<Vec<String>>();

    if args.len() != 3 {
        eprintln!("Usage: {} <year> <day>", args[0]);
        std::process::exit(1);
    }

    let year = args[1].parse::<u32>().unwrap();
    let day = args[2].parse::<u32>().unwrap();

    // Open the JSON file
    let file_path = "../rust-project.json";
    let mut file = OpenOptions::new().read(true).write(true).open(file_path)?;

    // Read the existing JSON content
    let mut content = String::new();
    file.read_to_string(&mut content)?;

    // Parse the JSON content into a Value
    let mut json_value: Value = serde_json::from_str(&content)?;

    // Insert a new JSON object
    let new_crate = json!({
        "root_module": format!("src/{}/Day{}/solve.rs", year, day),
        "edition": "2021",
        "deps": []
    });
    json_value["crates"].as_array_mut().unwrap().push(new_crate);

    // Serialize the updated JSON value back to a string
    let updated_content = serde_json::to_string_pretty(&json_value)?;

    // Truncate the file and write the updated content
    file.set_len(0)?;
    Seek::seek(&mut file, io::SeekFrom::Start(0))?;
    file.write_all(updated_content.as_bytes())?;

    Ok(())
}
