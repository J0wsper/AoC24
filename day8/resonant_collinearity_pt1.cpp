#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <memory>

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
    std::vector<std::pair<int, int>> locations;
    std::vector<std::pair<int, int>> antinodes;

public:
    frequency(char label, char_matrix* mat) {
        
        this->mat = std::make_shared<char_matrix>(mat);
        this->label = label;
        for (int i = 0; i < mat->get_height(); i++) {
            for (int j = 0; j < mat->get_width(); j++) {
                if (mat->get_by_id(i,j) == this->label) {
                    locations.push_back(std::pair<int, int>(i,j));
                }
            }
        }
        this->antinodes = std::vector<std::pair<int, int>>();
    }
    ~frequency() = default;

    // Just checks the validity of an antinode
    bool is_valid_antinode(std::pair<int, int> antinode) {
        return (antinode.first < mat->get_height() && antinode.second < mat->get_width()
                && antinode.first > 0 && antinode.second > 0);
    }

    // The real meat of the class. Calculates the positions of the antinodes 
    void find_antinodes() {

        // Looping through all the pairs
        std::vector<std::pair<int, int>>::iterator first_loc;
        for (first_loc = locations.begin(); first_loc != locations.end(); first_loc++) {
            std::vector<std::pair<int, int>>::iterator second_loc;
            for (second_loc = locations.begin(); second_loc != locations.end(); second_loc++) {
                
                // Do not want to calculate the pair of a location with itself
                if (first_loc == second_loc) {
                    continue;
                }

                // Getting the differences between the locations to calculate antinodes
                const int first_diff = second_loc->first - first_loc->first;
                const int second_diff = second_loc->second - first_loc->second;
                const std::pair<int, int> first_antinode(second_loc->first+first_diff, second_loc->second+second_diff);
                const std::pair<int, int> second_antinode(first_loc->first-first_diff, first_loc->second-second_diff);

                // Checking the antinodes are within the grid boundaries
                if (is_valid_antinode(first_antinode)) {
                    antinodes.push_back(first_antinode);
                }
                if (is_valid_antinode(second_antinode)) {
                    antinodes.push_back(second_antinode);
                }

            }
        }
    }

};

int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
