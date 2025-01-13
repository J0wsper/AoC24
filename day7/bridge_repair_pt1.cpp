#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <stdint.h>

class equation {
private:
    long test_val;
    std::vector<int> vals;
    bool is_possible;

public:

    // Produces an equation based on a line as a string
    equation(std::string eq) {
        vals = std::vector<int>();
        is_possible = false;
        std::string::iterator it = eq.begin();
        std::string::iterator prev = it;
        bool flag = false;
        while (it != eq.end()) {

            // We are still finding the test value
            if (*it == ':') {

                // We have to use long ints for some of the test values
                std::string test_val_str = std::string(prev, it);
                test_val = std::stol(test_val_str);
                it += 2;
                prev = it;
                flag = true;
                continue;
            }

            // We are looking for equation values
            if (flag && *it == ' ') {
                std::string val_str = std::string(prev, it);
                int val = std::stoi(val_str);
                vals.push_back(val);
                prev = it;
            }

            it++;
        }
        std::string val_str_last = std::string(prev, it);
        int val = std::stoi(val_str_last);
        vals.push_back(val);
    }

    ~equation() = default;

    int get_test_val() {
        return test_val;
    }

    // This is the real meat and potatoes of the class.
    // Returns whether the line is feasible with the operators + and *
    bool check_equation() {

        // This is kind of hacky but I thought it would be fun to mess with
        /* Essentially, operators is an unsigned integer where each digit (up to the length of the
        equation) represents either a multiplication or an addition in its binary representation.*/ 
        int len = vals.size();
        uint16_t mask = 1 << (len-1);
        uint16_t operators = 0;

        while (operators < mask) {
            long res = vals[0];
            for (int i = 1; i < len; i++) {
                if ((operators >> (len-1-i))%2 == 1) {
                    res *= vals[i];
                }
                else {
                    res += vals[i];
                }
            }
            if (res == test_val) {
                is_possible = true;
                break;
            }
            operators++;
        }
        
        return is_possible;
    }
};

int main(int argc, const char** argv) {
    
    std::ifstream file("day7.txt");
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1;
    }

    long res = 0;
    for (std::string line; std::getline(file, line);) {
        
        equation eq(line);
        if (eq.check_equation()) {
            res += eq.get_test_val();
        }

    }

    std::cout << "Total Calibration Result: " << res << std::endl;

    return 0;
}