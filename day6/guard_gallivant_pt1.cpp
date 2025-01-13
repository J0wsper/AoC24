#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

// Enum for the orientation of the guard
typedef enum orientation {
    NORTH,
    WEST,
    EAST,
    SOUTH
} orientation;

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

    // Finds the guard's starting position
    std::pair<int, int> find_guard() {
        std::vector<std::vector<char>>::iterator outer_it;
        for (outer_it = matrix.begin(); outer_it != matrix.end(); outer_it++) {
            
            std::vector<char>::iterator inner_it = std::find(outer_it->begin(), outer_it->end(), '^');

            // Found the guard
            if (inner_it != outer_it->end()) {
                int row_distance = std::distance(matrix.begin(), outer_it);
                int col_distance = std::distance(outer_it->begin(), inner_it);
                return std::pair<int, int>(row_distance, col_distance);
            }
        }

        // Return a sentinel value
        std::cerr << "Guard not present" << std::endl;
        return std::pair<int, int>(-1, -1);
    }

};

// Making a special class for the guard
class guard {
private:
    orientation facing;
    char_matrix* matrix;
    std::pair<int, int> pos;
    std::vector<std::pair<int, int>> visited;
public:

    guard(char_matrix* mat) {
        facing = NORTH;
        pos = mat->find_guard();
        matrix = mat;
        visited.push_back(pos);
    }
    ~guard() = default;

    orientation get_orientation() {
        return facing;
    }
    std::pair<int, int> get_pos() {
        return pos;
    }
    int get_visited() {
        return visited.size();
    }

    void rotate() {
        switch(facing) {
            case NORTH:
                facing = EAST;
                break;
            case SOUTH:
                facing = WEST;
                break;
            case WEST:
                facing = NORTH;
                break;
            case EAST:
                facing = SOUTH;
                break;
        }
    }

    void move() {
        std::vector<std::pair<int, int>>::iterator it;
        it = std::find(visited.begin(), visited.end(), pos);
        if (it == visited.end()) {
            visited.push_back(pos);
        }
        switch (facing) {
            case NORTH:
                pos.first--;
                break;
            case SOUTH:
                pos.first++;
                break;
            case WEST:
                pos.second--;
                break;
            case EAST:
                pos.second++;
                break;
        }
    }

    // The main method of the class. Moves the guard forward one space
    void increment() {
        std::pair<int, int> next_pos = pos;
        switch (facing) {
            case NORTH:
                next_pos.first--;
                break;
            case SOUTH:
                next_pos.first++;
                break;
            case WEST:
                next_pos.second--;
                break;
            case EAST:
                next_pos.second++;
                break;
        }
        char next_char = matrix->get_by_id(next_pos.first, next_pos.second);
        if (next_char == '#') {
            rotate();
        }
        move();
    }

};

int main(int argc, const char** argv) {

    std::ifstream file("day6.txt");
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1;
    }

    long res = 0;
    long count = 0;
    char_matrix mat = char_matrix(file);
    guard g(&mat);
    std::pair<int, int> guard_pos = mat.find_guard();

    // While the guard is on the screen:
    while (guard_pos.first < mat.get_height() && guard_pos.first > 0 &&
           guard_pos.second < mat.get_width() && guard_pos.second > 0) {
        
        // Increment the guard
        g.increment();
        guard_pos = g.get_pos();
        count++;
    }

    res = g.get_visited();

    std::cout << "Number of visited: " << res << std::endl;

    file.close();

    return 0;
}