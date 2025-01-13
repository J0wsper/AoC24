#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, const char** argv) {

    std::ifstream file("day3.txt");
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1;
    }

    long res = 0;
    for (std::string line; std::getline(file, line, '\n');) {

        int next_paranthesis_pos = 0;
        for (int i = 0; i < line.length(); i++) {

            // If we pass this, we found a candidate multiply
            if (line.substr(i, 4) != "mul(") {
                continue;
            }

            // Find the next parenthesis
            next_paranthesis_pos = i+4;
            while (line[next_paranthesis_pos] != ')' && next_paranthesis_pos < line.length()) {
                next_paranthesis_pos++;
            }

            // If the paranthesis are more than 8 characters apart, it cannot be a correct instruction
            const int len = next_paranthesis_pos-i-3;
            if (len > 8) {
                continue;
            }

            // Getting the candidate string
            std::string candidate_str = line.substr(i+4, len-1);
            std::string::iterator it;

            // Checking its validity with lexicographic comparison
            bool is_valid = true;
            for (it = candidate_str.begin(); it != candidate_str.end(); it++) {

                if (*it > '9' || (*it < '0' && *it != ',')) {
                    is_valid = false;
                    break;
                }

            }

            // Decipher the string because it passed all tests
            if (!is_valid) {
                continue;
            }

            // Break up the candidate string into its two components and add their product to res.
            const int comma_pos = candidate_str.find(',');
            const int first = std::stoi(candidate_str.substr(0, comma_pos));
            const int second = std::stoi(candidate_str.substr(comma_pos+1, len-1));
            res += first*second;

        }

    }

    std::cout << "Multiplication Result: " << res << std::endl;

    file.close();

    return 0;
}