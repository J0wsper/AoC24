use std::collections::VecDeque;
use std::{env, fs};

fn extract_halves(mut num: u64) -> (u64, u64) {
    // Catching bugs
    if num == 0 {
        return (0, 0);
    }
    let digits = num.ilog10() + 1;
    if digits == 1 {
        return (num, 0);
    }
    let mut first_half = 0;
    for index in 0..digits / 2 {
        let digit: u64 = num % 10;
        first_half += digit * 10_u64.pow(index);
        num /= 10;
    }
    let second_half = num;
    (first_half, second_half)
}

// The primary struct for this submission. Stores the stone line as a double-ended queue. This is
// because at every step, we are going to be popping off every value and replacing them with the
// new value after the blink.
#[derive(Debug)]
struct StoneLine {
    _data: String,
    stone_line: VecDeque<u64>,
}

impl StoneLine {
    fn new(data: String) -> Self {
        StoneLine {
            _data: String::from(data.trim_ascii()),
            stone_line: {
                let data_vec: VecDeque<&str> = data.trim_ascii().split(' ').collect();
                let mut stone_line = VecDeque::<u64>::new();
                for str in data_vec.into_iter() {
                    let len = str.len();
                    let mut val: u64 = 0;
                    for (i, ch) in str.chars().enumerate() {
                        let exp: u32 = (len - i - 1).try_into().expect(
                            "Could not convert into exponent in stone line initialization.",
                        );
                        let sig = <u32 as Into<u64>>::into(
                            ch.to_digit(10)
                                .expect("Could not convert digts in stone line initialization."),
                        );
                        val += sig * 10_u64.pow(exp);
                    }
                    stone_line.push_back(val);
                }
                stone_line
            },
        }
    }
    fn step(&mut self) {
        let mut new_line = VecDeque::new();
        while !self.stone_line.is_empty() {
            let val = self
                .stone_line
                .pop_back()
                .expect("Could not pop value off of stone line");
            if val == 0 {
                new_line.push_back(1);
            } else if val.ilog10() % 2 == 0 {
                let halves = extract_halves(val);
                new_line.push_back(halves.0);
                new_line.push_back(halves.1);
            } else {
                new_line.push_back(val * 2024);
            }
        }
        self.stone_line = new_line;
    }
}

impl StoneLine {}

fn main() -> std::io::Result<()> {
    env::set_var("RUST_BACKTRACE", "1");

    let data = fs::read_to_string("day11.txt").expect("Could not read file.");
    let mut stone_line = StoneLine::new(data);
    dbg!(&stone_line);
    stone_line.step();
    dbg!(&stone_line);
    Ok(())
}
