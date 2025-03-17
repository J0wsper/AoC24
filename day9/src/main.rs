use std::{env, fs};
use std::collections::*;

// Sentinel value for free spaces.
const FREE_SPACE: u32 = u32::MAX;

//Used in the seek method to show which way we are seeking
enum Direction {
    FORWARD,
    BACKWARD,
}

struct Disk<'a> {
    data: &'a str,
}

impl<'a> Disk<'a> {
    const fn new(data: &'a str) -> Self {
        Self {
            data: data.trim_ascii(),
        }
    }

    fn load_disk(&self) -> Vec<u32> {

        // Loops through each character in the file
        let mut disk = Vec::new();
        for (i, c) in self.data.chars().enumerate() {

            // Loops through and adds the given number/index the correct number of times
            for _ in 0..c.to_digit(10).unwrap() {

                // Either pushes a free space or an unwrapped index
                disk.push(
                    if i % 2 == 1 {
                        FREE_SPACE
                    }
                    else {
                        (i as u32)/2
                    }
                );
            }
        }

        disk
    }

    // Computes the checksum of a formatted disk and outputs as a 64-bit number
    fn compute_checksum(disk: &[u32]) -> u64 {

        let mut checksum: u64 = 0;
        
        // Simple loop to iterate while keeping track of position
        for (i, val) in disk.iter().enumerate() {
            if val != &FREE_SPACE {
                checksum += u64::from(*val)*(i as u64);
            }
        }

        checksum
    }

    fn reformat_disk_day1(&self) -> u64 {

        // Loads the disk
        let mut vec: Vec<u32> = self.load_disk();

        // We are going to approaching both ends of the vec
        let mut i: usize = 0;
        let mut j: usize = vec.len()-1;

        // Doing this in a kind of dumb, nested-loop way
        while i < j {

            // Finding free spaces from the front
            while vec[i] != FREE_SPACE && i < j {
                i += 1;
            }

            // Finding blocks from the back
            while vec[j] == FREE_SPACE && i < j {
                j -= 1;
            }

            // Swapping the two
            vec.swap(i,j);
        }

        // Ending by computing the checksum of the disk
        Self::compute_checksum(&vec)
    }

    // Attempts to find either a contiguous free block or a contiguous file in disk
    fn seek(&self, disk: &Vec<u32>, start: usize, dir: Direction) -> usize {

        let mut loc: usize = start;
        match dir {
            Direction::FORWARD => {
                while disk[loc] == FREE_SPACE && loc < disk.len()-1 {
                    loc += 1;
                    // println!("Stuck in seek");
                }
            },
            Direction::BACKWARD => {
                while disk[loc] == disk[start] && loc > 0 {
                    loc -= 1;
                }
            },
        };

        loc
    }

    fn reformat_disk_day2(&self) -> u64 {

        let mut vec: Vec<u32> = self.load_disk();

        // However, we will now check the size of the free space before moving.
        let mut j: usize  = vec.len() - 1;
        let mut seen: HashSet<u32> = HashSet::new();

        // j tracks where we have attempted moving blocks so far.
        while j > 0 {

            // Finding the rightmost unmoved block
            while vec[j] == FREE_SPACE {
                j -= 1;
            }

            // Marking that we have seen this new value

            // Finding the length of that unmoved block
            let k = self.seek(&vec, j, Direction::BACKWARD); 
            let block_size = j-k;

            // If we have seen the value before, continue
            let val = vec[j];
            if !seen.insert(val) {
                j = k;
                continue;
            }

            // Seeking from left to right, finding a free space that can accomodate
            let mut i: usize = 0;
            let mut is_moved = false;
            while i < k && !is_moved{

                // Finding the length of free space
                while vec[i] != FREE_SPACE && i < k {
                    i += 1;
                }
                let l = self.seek(&vec, i, Direction::FORWARD);
                let free_size = l-i;
                

                // If the free space is not big enough, continue
                if free_size < block_size {
                    i = l;
                    continue;
                }

                // If it is, move that whole block
                for _ in 0..block_size {
                    vec.swap(i, j);
                    i += 1;
                    j -=1;
                }

                // Marking that we have moved our block
                is_moved = true;

            }

        }

        Self::compute_checksum(&vec)

    }

}

fn main() -> std::io::Result<()> {

    env::set_var("RUST_BACKTRACE", "1");

    let data = fs::read_to_string("day9.txt")
        .expect("Could not read file");
    let disk = Disk::new(&data);
    disk.load_disk();
    println!("The solution to day1 is {}", disk.reformat_disk_day1());
    println!("The solution to day2 is {}", disk.reformat_disk_day2());

    Ok(())
}
