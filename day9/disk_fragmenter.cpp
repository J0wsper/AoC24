#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <algorithm>

class disk {
private:
    std::list<int> contents;
    std::list<int> parsed_input; 

public:
    disk(std::ifstream& file) {

        // Initializing containers
        contents = std::list<int>();
        parsed_input = std::list<int>();

        // Casting each individual character in the file to an int
        char ch;
        while (file >> ch) {
            int val = (int) ch;
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
};

int main(int argc, char const *argv[])
{
    return 0;
}
