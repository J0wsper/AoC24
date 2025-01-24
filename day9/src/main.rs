use std::fs;

// Sentinel value for free spaces.
const FREE_SPACE: u32 = u32::MAX;

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
                        u32::try_from(i).unwrap()/2
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

    fn reformat_disk(&self) -> u64 {

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

}

fn main() -> std::io::Result<()> {

    let data = fs::read_to_string("src/day9.txt")
        .expect("Could not read file");
    let disk = Disk::new(&data);
    disk.load_disk();
    println!("The solution is {}", disk.reformat_disk());

    Ok(())
}