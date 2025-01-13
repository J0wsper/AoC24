#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>

int main() {

    // Safe count determines how many safe lines we have seen so far
    int safe_count = 0;
    std::ifstream file("day2.txt");

    // File failed to open.
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1; 
    }

    // Otherwise, get each line
    for (std::string line; std::getline(file, line, '\n');) {

        // Looping through the line
        std::vector<int> arr;
        int prev = 0;

        // Creating an array of integers from each line
        for (int i = 0; i < line.length(); i++) {

            // If we encounter a space or reach the end of the line, add a new integer to our array
            // according to when we hit the end of the last one
            if (line[i] == ' ' || i == line.length()-1) {
                const int val = std::stoi(line.substr(prev, i));
                arr.push_back(val);
                prev = i+1;
            }
        }

        // Removing a single element from the array. If we get a single success, break.
        bool is_valid;

        for (int j = 0; j < arr.size(); j++) {

            std::vector<int> sub_arr = arr;
            sub_arr.erase(sub_arr.begin()+j);

            // Checking the first two values and making our comparison
            // is_less determines if the relationship between two consecutive elements is less than or greater than
            // is_valid determines if the line is valid or not
            bool is_less;
            is_valid = true;

            // Determining if the sequence is monotonic increasing or decreasing based on the first two
            // elements of the sequence
            if (sub_arr[0] < sub_arr[1]) {
                is_less = true;
            }
            else if (sub_arr[0] > sub_arr[1]) {
                is_less = false;
            }
            else {
                is_valid = false;
            }

            for (int k = 1; k < sub_arr.size(); k++) {

                // Checking that the monotonic relationship is preserved throughout the elements
                if (sub_arr[k-1] < sub_arr[k] != is_less || sub_arr[k-1] == sub_arr[k]) {
                    is_valid = false;
                }

                // Making sure the difference between consecutive elements is less than or equal to 3
                if (abs(sub_arr[k] - sub_arr[k-1]) > 3) {
                    is_valid = false;
                }
            }

            // If the sequence is invalid for any reason, skip this line and do not add it to the safe count
            if (is_valid) {
                break;
            }

        }
        if (is_valid) {
            safe_count++;
        }

    }

    std::cout << "Safe Count: " << safe_count << std::endl;

    // Close the file when we are done
    file.close();

    return 0;
}