#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <algorithm>
#include <set>

// Reimplenting the char_matrix from day 4 bc I think it will be helpful
class char_matrix {
private:
    std::vector<std::vector<char>> matrix;
    int m;
    int n;

public:
    char_matrix(std::ifstream& file) {

        bool first = true;
        for (std::string line; std::getline(file, line, '\n');) {
            std::string::iterator it;
            std::vector<char> row_vec;
            for (it = line.begin(); it != line.end(); it++) {
                row_vec.push_back(*it);
            }
            if (first) {
                this->m = row_vec.size();
                first = false;
            }
            else if (!first && row_vec.size() != this->m) {
                std::cout << "uh oh! stinky!" << std::endl;
            }
            this->matrix.push_back(row_vec);
        }
        this->n = this->matrix.size();
    }

    ~char_matrix() = default;

    int get_width() {
        return n;
    }

    int get_height() {
        return m;
    }

    // Returns the character at a given row and column
    char get_by_id(int row, int col) {
        if (row > m-1 || col > n-1 ||
            row < 0 || col < 0) {
            return '\0';
        }
        return this->matrix[row][col];
    }

};

class frequency {
private:
    char label;
    std::shared_ptr<char_matrix> mat;
    std::set<std::pair<int, int>> locations;
    std::set<std::pair<int, int>> antinodes;

public:
    frequency(char label, char_matrix& mat) {
        
        this->mat = std::make_shared<char_matrix>(mat);
        this->label = label;
        for (int i = 0; i < this->mat->get_height(); i++) {
            for (int j = 0; j < this->mat->get_width(); j++) {
                if (this->mat->get_by_id(i,j) == this->label) {
                    locations.insert(std::pair<int, int>(i,j));
                }
            }
        }
        this->antinodes = std::set<std::pair<int, int>>();
    }
    ~frequency() = default;

    // Just checks the validity of an antinode
    bool is_valid_antinode(std::pair<int, int> antinode) {
        return (antinode.first < mat->get_height() && antinode.second < mat->get_width()
                && antinode.first >= 0 && antinode.second >= 0);
    }

    int get_antinodes_size() {
        return this->antinodes.size();
    }

    char get_label() {
        return this->label;
    }

    // The real meat of the class. Calculates the positions of the antinodes 
    void find_antinodes() {

        // Looping through all the pairs
        std::set<std::pair<int, int>>::iterator first_loc;
        for (first_loc = locations.begin(); first_loc != locations.end(); first_loc++) {
            std::set<std::pair<int, int>>::iterator second_loc;
            for (second_loc = locations.begin(); second_loc != locations.end(); second_loc++) {
                
                // Do not want to calculate the pair of a location with itself
                if (first_loc == second_loc) {
                    continue;
                }

                // Getting the differences between the locations to calculate antinodes
                int first_diff = second_loc->first - first_loc->first;
                int second_diff = second_loc->second - first_loc->second;
                std::pair<int, int> first_antinode(second_loc->first+first_diff, second_loc->second+second_diff);
                std::pair<int, int> second_antinode(first_loc->first-first_diff, first_loc->second-second_diff);

                // Checking the antinodes are within the grid boundaries
                if (is_valid_antinode(first_antinode)) {
                    antinodes.insert(first_antinode);
                }
                if (is_valid_antinode(second_antinode)) {
                    antinodes.insert(second_antinode);
                }

            }
        }
    }

    // Checks the antinodes of both frequencies and removes antinodes from this if there are overlaps
    void compare_antinodes(const frequency* freq) {
        std::set<std::pair<int, int>> new_antinodes;

        std::set_difference(
            antinodes.begin(), antinodes.end(), 
            freq->antinodes.begin(), freq->antinodes.end(), 
            std::inserter(new_antinodes, new_antinodes.end())
        );

        if (new_antinodes != antinodes) {
            antinodes = new_antinodes;
        }
    }
};

int main(int argc, char const *argv[])
{
    std::ifstream file("day8.txt");
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1;
    }

    std::set<char> seen_chars;
    char ch;
    long res = 0;

    // Finding all the unique characters
    while (file >> ch) {
        if (!isspace(ch) && ch != '.') {
            seen_chars.insert(ch);
        }
    }

    // Resetting the file's flags and making the character matrix
    file.clear();
    file.seekg(0);
    char_matrix mat(file);

    // Creating the frequencies
    std::set<char>::iterator set_it;
    std::vector<frequency> freq_vec;
    for (set_it = seen_chars.begin(); set_it != seen_chars.end(); set_it++) {
        frequency freq(*set_it, mat);
        freq.find_antinodes();
        freq_vec.push_back(freq);
    }

    // Finding their antinodes
    std::vector<frequency>::iterator outer_vec_it;
    for (outer_vec_it = freq_vec.begin(); outer_vec_it != freq_vec.end(); outer_vec_it++) {
        std::vector<frequency>::iterator inner_vec_it;
        
        // Comparing antinodes to get rid of superfluous ones
        for (inner_vec_it = freq_vec.begin(); inner_vec_it != freq_vec.end(); inner_vec_it++) {
            if (outer_vec_it->get_label() == inner_vec_it->get_label()) {
                continue;
            }
            outer_vec_it->compare_antinodes(&(*inner_vec_it));
        }
        res += outer_vec_it->get_antinodes_size();   
    }

    std::cout << "Total Number of Antinodes: " << res << std::endl;

    file.close();

    return 0;
}
