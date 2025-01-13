#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

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
        std::vector<std::pair<int, int>>::iterator it;
        for (it = ret.begin(); it != ret.end();) {
            std::pair<int, int> opp1(-1*it->first, it->second);
            std::pair<int, int> opp2(it->first, -1*it->second);
            std::vector<std::pair<int, int>>::iterator pair1 = std::find(ret.begin(), ret.end(), opp1);
            std::vector<std::pair<int, int>>::iterator pair2 = std::find(ret.begin(), ret.end(), opp2);
            if (pair1 == ret.end() && pair2 == ret.end()) {
                it = ret.erase(it);
            }
            else {
                it++;
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

    std::vector<std::string> check_opposites(int row, int col, std::vector<std::pair<int, int>> offsets) {

        std::vector<std::string> ret;

        for (int i = 0; i < offsets.size(); i++) {

            std::string curr_string;

            int row_offset = offsets[i].first;
            int col_offset = offsets[i].second;

            int row_off_opp = -1*row_offset;
            int col_off_opp = -1*col_offset;

            char offset_char = this->get_by_id(row+row_offset, col+col_offset);
            char center_char = this->get_by_id(row, col);
            char opposite_char = this->get_by_id(row+row_off_opp, col+col_off_opp);

            curr_string.push_back(offset_char);
            curr_string.push_back(center_char);
            curr_string.push_back(opposite_char);

            ret.push_back(curr_string);

        }

        return ret;

    }

};

int binom(int n, int k) {
    if (k > n) {
        return 0;
    }
    if (k == 0 || k == n) {
        return 1;
    }
    return binom(n-1,k-1)+binom(n-1,k);
}

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
            if (mat.get_by_id(i, j) != 'A') {
                continue;
            }

            // Otherwise, check its adjacent spots
            std::vector<std::pair<int, int>> wide_candidates = mat.check_wide(i,j,'M');

            // If there are no adjacent Ms, continue
            if (wide_candidates.size() == 0) {
                continue;
            }

            // If there are, check them according to their relative position from our current query
            std::vector<std::string> opposite_candidates = mat.check_opposites(i, j, wide_candidates);

            // Count the number of SAM or MAS strings
            int num_sam = 0;
            for (int k = 0; k < opposite_candidates.size(); k++) {
                if (opposite_candidates[k] == "SAM" || opposite_candidates[k] == "MAS") {
                    num_sam++;
                }
            }
            ret += binom(num_sam, 2);
        }
    }

    std::cout << "X-MAS Amount: " << ret << std::endl;

    file.close();

    return 0;
}
