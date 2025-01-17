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

    // Finds the soonest free block
    void find_free_space(std::list<int>::iterator& first, std::list<int>::iterator& last) {
        first = std::find(contents.begin(), contents.end(), -1);
        last = std::find_if(first, contents.end(), [](int q){return q != -1;});
    }

    // Finds the last contiguous block
    void find_last_block(std::list<int>::reverse_iterator& rfirst, std::list<int>::reverse_iterator& rend) {
        rfirst = std::find_if(contents.rbegin(), contents.rend(), [](int p){return p != -1;});
        rend = std::find(rfirst, contents.rend(), -1);
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
    void compact() {
        
        // TODO: This needs to be finished.
        std::list<int>::iterator curr = std::find(contents.begin(), contents.end(), -1);
        std::list<int>::reverse_iterator rcurr = std::find_if(contents.rbegin(), contents.rend(), [](int p){return p != -1;});
        while (curr != contents.end() && rcurr != contents.rend()) {
            contents.emplace(curr, *rcurr);
            contents.erase(curr);
            curr = std::find(curr, contents.end(), -1);
            rcurr = std::find_if(rcurr, contents.rend(), [](int p){return p != -1;});
        }

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
