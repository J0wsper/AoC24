#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <vector>
#include <algorithm>

class disk {
private:
    std::list<int> contents;
    std::list<int> parsed_input; 
    bool is_compacted;
    long checksum;

    // Wrappers for function calls I use a lot
    std::list<int>::reverse_iterator find_block_back() {
        return std::find_if(contents.rbegin(), contents.rend(), [](int p){return p != -1;});
    }

    std::list<int>::iterator find_free_front() {
        return std::find(contents.begin(), contents.end(), -1);
    }

    std::list<int>::iterator find_next_block_front(std::list<int>::iterator curr) {
        return std::find_if(curr, contents.end(), [](int p){return p != -1;});
    }

    std::list<int>::reverse_iterator find_next_free_back(std::list<int>::reverse_iterator curr) {
        return std::find(curr, contents.rend(), -1);
    }

public:

    // Initializes an uncompacted disk from a given file.
    // NOTE: This all works perfectly. There are bugs further down in the code.
    disk(std::ifstream& file) {

        // Initializing containers
        contents = std::list<int>();
        parsed_input = std::list<int>();
        is_compacted = false;
        checksum = 0;

        // Casting each individual character in the file to an int
        char ch;
        while (file >> ch) {
            int val = (int) ch - '0';
            parsed_input.push_back(val);
        }

        // Turning the parsed input into the actual disk contents
        bool is_space = false;
        bool is_block = true;
        long index = 0;
        std::list<int>::const_iterator it;
        std::list<int>::iterator curr = contents.begin();
        for (it = parsed_input.cbegin(); it != parsed_input.cend(); it++) {
            
            // If we have a space, add a certain number of a sentinel value
            if (is_space) {

                std::list<int> arr(*it, -1);
                contents.splice(contents.end(), arr);
                is_space = false;
                is_block = true;
            }

            // If we have a block, add a certain number of our index number
            else if (is_block) {

                std::list<int> arr(*it, index);
                contents.splice(contents.end(), arr);
                is_block = false;
                is_space = true;
                index++;
            }
        }

    } 

    ~disk() = default;

    // Compacts the disk
    // TODO: This is still unfinished
    void compact() {
        
        // Finding the soonest block and free space from the back and the front respectively
        auto block_begin = find_block_back();
        auto free_begin = find_free_front();
        auto block_end = find_next_free_back(block_begin);
        auto free_end = find_next_block_front(free_begin);

        // Finding whichever one is smaller
        int free_size = std::distance(free_begin, free_end);
        int block_size = std::distance(block_begin, block_end);
       
        // Looping through until we have compacted the whole disk
        // while (free_end != contents.end() || block_end != contents.rend()) {
            
        // }

        // Setting the compacted flag as true
        is_compacted = true;
    }

    // Calculates the checksum of a compacted disk
    long calculate_checksum() {
        if (!is_compacted) {
            std::cerr << "Disk is not yet compacted" << std::endl;
            return -1;
        }
        long curr = 0;
        std::list<int>::iterator it;
        for (it = contents.begin(); it != contents.end(); it++) {
            checksum += curr*(*it);
        }
        return checksum;
    }
};

int main(int argc, char const *argv[])
{
    // Just some testing code to understand how std::swap_ranges works
    // std::list<int> arr1 = {3,3,3,-1,-1,-1,4,4,4,4};
    // std::list<int>::iterator free_space_begin = std::find(arr1.begin(), arr1.end(), -1);
    // std::list<int>::iterator free_space_end = std::find(arr1.begin(), arr1.end(), 4);
    // std::swap_ranges(free_space_begin, free_space_end, arr1.rbegin());

    std::ifstream file("day9.txt");
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1;
    }

    disk d(file);
    d.compact();

    std::cout << "Total Checksum: " << d.calculate_checksum() << std::endl;

    return 0;
}
