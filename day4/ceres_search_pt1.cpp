#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>

class char_matrix {
public:

    std::vector<std::vector<char>> matrix;
    int m;
    int n;

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

    char get_by_id(int row, int col) {
        if (row > m-1 || col > n-1 ||
            row < 0 || col < 0) {
            return '\0';
        }
        return this->matrix[row][col];
    }

    // Checks all the positions surrounding (row, col) for search
    std::vector<std::pair<int, int>> check_wide(int row, int col, char search) {

        std::vector<std::pair<int, int>> ret;
        for (int a = -1; a <= 1; a++) {
            for (int b = -1; b <= 1; b++) {
                if (a == 0 && b == 0) {
                    continue;
                }
                char cand = this->get_by_id(row+a, col+b);
                if (cand == search) {
                    std::pair<int, int> pos(a, b);
                    ret.push_back(pos);
                }
            }
        }

        return ret;
    }

    // Checks a line of length len based on relative positions (a,b) from (row, col)
    std::vector<std::string> check_deep(int row, int col, int len, std::vector<std::pair<int, int>> offsets) {

        std::vector<std::string> ret;
        for (int i = 0; i < offsets.size(); i++) {

            std::string curr_string;
            for (int j = 0; j < len; j++) {

                int row_offset = offsets[i].first;
                int col_offset = offsets[i].second;

                char next_char = this->get_by_id(row+j*row_offset, col+j*col_offset);
                if (next_char == '\0') {
                    break;
                }
                curr_string.push_back(next_char);

            }
            ret.push_back(curr_string);

        }

        return ret;
    }

};

int main(int argc, char const *argv[])
{
    
    int ret = 0;

    std::ifstream file("day4.txt");
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1;
    }

    char_matrix mat = char_matrix(file);

    // Looping through rows
    for (int i = 0; i < mat.m; i++) {

        // Looping through columns
        for (int j = 0; j < mat.n; j++) {

            // If we haven't found the start of a candidate, skip it
            if (mat.get_by_id(i, j) != 'X') {
                continue;
            }

            // Otherwise, check its adjacent spots
            std::vector<std::pair<int, int>> wide_candidates = mat.check_wide(i,j,'M');

            // If there are no adjacent Ms, continue
            if (wide_candidates.size() == 0) {
                continue;
            }

            // If there are, check them according to their relative position from our current query
            std::vector<std::string> deep_candidates = mat.check_deep(i, j, 4, wide_candidates);

            // Count the number of XMAS strings
            for (int k = 0; k < deep_candidates.size(); k++) {
                if (deep_candidates[k] != "XMAS") {
                    continue;
                }
                ret++;
            }
        }
    }

    std::cout << "XMAS Amount: " << ret << std::endl;

    file.close();

    return 0;
}
