use std::{collections::HashSet, env, fs};

// Important consts. If the file is changed, update MAP_WIDTH and MAP_HEIGHT 
// with the number of columns and the number of rows respectively.
const UNASSIGNED_32: u32 = u32::MAX;
const MAP_WIDTH: usize = 45;
const MAP_HEIGHT: usize = 45;

// A struct used for the seek method
struct Directions{
    north: bool,
    east: bool,
    south: bool,
    west: bool,
}

// The primary struct to solve this problem
#[derive(Debug)]
struct TopographicMap {
    _data: String,
    char_mat: [[u32; MAP_WIDTH]; MAP_HEIGHT],
}

impl TopographicMap {
    fn new(data: &str) -> Self {
        Self {
            _data: String::from(data.trim_ascii()),
            char_mat: {

                let mut char_mat: [[u32; MAP_WIDTH]; MAP_HEIGHT] = [[UNASSIGNED_32; MAP_WIDTH]; MAP_HEIGHT];

                // Converting each line to an array of characters
                for (index, line) in data.lines().enumerate() {

                    // Converting a line to a vector of characters
                    let row_vec: Vec<u32> = line.chars().flat_map(|ch| ch.to_digit(10)).collect();

                    // Converting that vector into an array.
                    let row_arr: [u32; MAP_WIDTH] = row_vec.try_into()
                        .expect("Could not convert row vector into row array.");

                    // Pushing the row onto the character matrix
                    char_mat[index] = row_arr;

                }

                char_mat
            }
        }
    }

    // Given a coordinate, returns an array of adjacent points that you can go to.
    fn find_directions(&self, start: (usize, usize)) -> Directions {

        let start_val = self.char_mat[start.0][start.1];

        // We can make this code slightly more concise because Rust && evaluations
        // short circuit if the first condition is false, meaning we do not need to
        // worry about the second condition being out-of-bounds.
        Directions {
            north: start.0 > 0 && self.char_mat[start.0-1][start.1] == start_val+1,
            east: start.1 < MAP_WIDTH-1 && self.char_mat[start.0][start.1+1] == start_val+1,
            south: start.0 < MAP_HEIGHT-1 && self.char_mat[start.0+1][start.1] == start_val+1,
            west: start.1 > 0 && self.char_mat[start.0][start.1-1] == start_val+1,
        }
    }

    // Given a trailhead, will return that trail's score
    fn find_trails(&self, start: (usize, usize), peaks: &mut HashSet<(usize, usize)>) -> u32 {

        // Base case
        if self.char_mat[start.0][start.1] == 9 && peaks.insert(start) {
            return 1;
        }

        // Finding possible directions
        let directions = self.find_directions(start);

        // Recursive case depending on which directions are valid
        let mut trail_score = 0;
        if directions.north {trail_score += self.find_trails((start.0-1, start.1), peaks);}
        if directions.east {trail_score += self.find_trails((start.0, start.1+1), peaks);}
        if directions.south {trail_score += self.find_trails((start.0+1,start.1), peaks);}
        if directions.west {trail_score += self.find_trails((start.0, start.1-1), peaks);}

        trail_score
    }
    
    pub fn part1(&self) -> u32 {

        let mut total_trail_score = 0;

        // For each character, test if it is is a trail head. If it is, find trails
        // on that trail head.
        for (i, row) in self.char_mat.iter().enumerate() {
            for (j, val) in row.iter().enumerate() {
                if *val == 0 {

                    // Peaks is a set for each trailhead to keep track of which peaks
                    // we have visited so far to prevent overcounting.
                    let mut peaks: HashSet<(usize, usize)> = HashSet::new();
                    total_trail_score += self.find_trails((i,j), &mut peaks);
                }
            }
        }

        total_trail_score
    }
}

fn main() -> std::io::Result<()> {
    
    env::set_var("RUST_BACKTRACE", "1");

    let data = fs::read_to_string("day10.txt")
        .expect("Could not read day10.txt.");

    let test: TopographicMap = TopographicMap::new(&data);

    // dbg!(&test);

    // dbg!(&test.char_mat[0][1]);

    println!("The total trail score is {}", test.part1());

    // 1062: Too high

    Ok(())
}
