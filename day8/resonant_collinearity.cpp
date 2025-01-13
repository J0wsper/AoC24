#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

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
    std::vector<std::pair<int, int>> locations;
    std::vector<std::pair<int, int>> antinodes;

public:
    frequency(char label, char_matrix mat) {
        
        this->label = label;
        for (int i = 0; i < mat.get_height(); i++) {

            for (int j = 0; j < mat.get_width(); j++) {
                if (mat.get_by_id(i,j) == label) {
                    
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
