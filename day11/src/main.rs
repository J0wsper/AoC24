use std::collections::{BTreeMap, VecDeque};
use std::{env, fs};

fn extract_halves(mut num: u128) -> (u128, u128) {
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
        let digit: u128 = num % 10;
        first_half += digit * 10_u128.pow(index);
        num /= 10;
    }
    let second_half = num;
    (first_half, second_half)
}

// The primary struct for this submission. Stores the stone line as a double-ended queue. This is
// because at every step, we are going to be popping off every value and replacing them with the
// new value after the blink.
#[derive(Debug)]
struct StoneLine1 {
    _data: String,
    stone_line: VecDeque<u128>,
}

// NOTE: This is the day 1 struct
impl StoneLine1 {
    fn new(data: String) -> Self {
        StoneLine1 {
            _data: String::from(data.trim_ascii()),
            stone_line: {
                let data_vec: VecDeque<&str> = data.trim_ascii().split(' ').collect();
                let mut stone_line = VecDeque::<u128>::new();
                for str in data_vec.into_iter() {
                    let len = str.len();
                    let mut val: u128 = 0;
                    for (i, ch) in str.chars().enumerate() {
                        let exp: u32 = (len - i - 1).try_into().expect(
                            "Could not convert into exponent in stone line initialization.",
                        );
                        let sig = <u32 as Into<u128>>::into(
                            ch.to_digit(10)
                                .expect("Could not convert digts in stone line initialization."),
                        );
                        val += sig * 10_u128.pow(exp);
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
                .pop_front()
                .expect("Could not pop value off of stone line");
            if val == 0 {
                new_line.push_back(1);
            } else if (val.ilog10() + 1) % 2 == 0 {
                let halves = extract_halves(val);
                new_line.push_back(halves.1);
                new_line.push_back(halves.0);
            } else {
                new_line.push_back(val * 2024);
            }
        }
        self.stone_line = new_line;
    }
}

#[derive(PartialOrd, Ord, PartialEq, Eq, Debug)]
enum StoneType {
    Zero,
    EvenDigits,
    OddDigits,
}

struct StoneLine2 {
    _data: String,
    stone_line: BTreeMap<StoneType, u128>,
}

impl StoneLine2 {
    fn new(data: String) -> Self {
        StoneLine2 {
            _data: String::from(data.trim_ascii()),
            stone_line: {
                let data_vec: VecDeque<&str> = data.trim_ascii().split(' ').collect();
                let mut stone_line = BTreeMap::<StoneType, u128>::new();
                stone_line.insert(StoneType::Zero, 0);
                stone_line.insert(StoneType::EvenDigits, 0);
                stone_line.insert(StoneType::OddDigits, 0);
                for str in data_vec.into_iter() {
                    let len = str.len();
                    let mut val: u128 = 0;
                    for (i, ch) in str.chars().enumerate() {
                        let exp: u32 = (len - i - 1).try_into().expect(
                            "Could not convert into exponent in stone line initialization.",
                        );
                        let sig = <u32 as Into<u128>>::into(
                            ch.to_digit(10)
                                .expect("Could not convert digts in stone line initialization."),
                        );
                        val += sig * 10_u128.pow(exp);
                    }
                    if val == 0 {
                        stone_line.insert(
                            StoneType::Zero,
                            stone_line
                                .get(&StoneType::Zero)
                                .expect("Could not read zeroes in map.")
                                + 1,
                        );
                    } else if (val.ilog10() + 1) % 2 == 0 {
                        stone_line.insert(
                            StoneType::EvenDigits,
                            stone_line
                                .get(&StoneType::EvenDigits)
                                .expect("Could not read even digits in map")
                                + 1,
                        );
                    } else {
                        stone_line.insert(
                            StoneType::OddDigits,
                            stone_line
                                .get(&StoneType::OddDigits)
                                .expect("Could not read odd digits in map")
                                + 1,
                        );
                    }
                }
                stone_line
            },
        }
    }

    fn step(&mut self) {
        let zeroes = self
            .stone_line
            .get(&StoneType::Zero)
            .expect("Could not read zeroes in map");
        let evens = self
            .stone_line
            .get(&StoneType::EvenDigits)
            .expect("Could not read even digits in map");
        let odds = self
            .stone_line
            .get(&StoneType::OddDigits)
            .expect("Could not read odd digits in map");
    }
}

fn main() -> std::io::Result<()> {
    env::set_var("RUST_BACKTRACE", "1");

    let data = fs::read_to_string("day11.txt").expect("Could not read file.");
    let mut stone_line = StoneLine1::new(data);

    // NOTE: Computationally-infeasible to use this approach for the 75 step thing.
    // Maybe we can count the number of 0s, even-digited numbers and odd-digited numbers?
    // This would make our life a bit easier and probably much faster.
    for _ in 0..50 {
        stone_line.step();
    }
    let length = stone_line.stone_line.len();
    print!("{length}");
    Ok(())
}
