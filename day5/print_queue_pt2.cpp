#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <memory>
#include <map>
#include <algorithm>

bool not_perm(std::pair<int, std::pair<bool, bool>> val) {
    return val.second.second;
}

typedef struct node {
    int val;
    bool perm_mark;
    bool temp_mark;
} node;

class graph {
private:

    // Vals contains the integers we care about for this particular graph
    // Adj_list is the adjacency list of the resultant graph
    // Mark_list is used for depth-first search
    std::vector<int> vals;
    std::map<int, std::vector<int>> adj_list;
    std::map<int, std::pair<bool, bool>> mark_list;
    int size;

public:

    graph(std::vector<std::string> rules, std::string values) {
        size = 0;
        vals = std::vector<int>();
        load_vals(values);
        adj_list = std::map<int, std::vector<int>>();
        mark_list = std::map<int, std::pair<bool, bool>>();
        for (int i = 0; i < vals.size(); i++) {
            adj_list[vals[i]] = std::vector<int>();
        }
        for (int j = 0; j < rules.size(); j++) {
            add_edge(rules[j]);
        }
    }

    ~graph() = default;

    void load_vals(std::string values) {
        int prev = 0;
        for (int i = 0; i <= values.size(); i++) {
            if (values[i] == ',' || i == values.size()) {
                int val = std::stoi(values.substr(prev, i));
                vals.push_back(val);
                prev = i+1;
                size++;
            }
        }
    }

    void add_edge(std::string rule) {

        int out = std::stoi(rule.substr(0, 2));
        int in = std::stoi(rule.substr(3, 5));

        // We do not care about this rule
        if (std::find(vals.begin(), vals.end(), out) == vals.end() ||
            std::find(vals.begin(), vals.end(), in) == vals.end()) {
            return;
        }
        
        // If it passes the test, we do care about this rule and we add it to the graph
        adj_list[out].push_back(in);
        if (adj_list[out].size() == 1) {
            mark_list[out].first, mark_list[out].second = false;
        }

    }

    void mark_temp(int v) {
        mark_list[v].first = true;
    }

    void mark_perm(int v) {
        mark_list[v].second = true;
    }

    std::vector<int> get_vals() {
        return vals;
    }

    void visit(int v, std::deque<int>* ret) {

        if (mark_list[v].second) {
            return;
        }
        else if (mark_list[v].first) {
            std::cerr << "Encountered temporary mark." << std::endl;
            ret = nullptr;
            return;
        }

        mark_temp(v);

        for (int i = 0; i < adj_list[v].size(); i++) {
            visit(adj_list[v][i], ret);
        }
        
        mark_perm(v);

        ret->push_front(v);
    }

    std::deque<int>* dfs() {
        std::deque<int>* ret = new std::deque<int>;
        std::map<int, std::pair<bool, bool>>::iterator cand;
        cand = std::find_if_not(mark_list.begin(), mark_list.end(), not_perm);
        while (cand != mark_list.end()) {
            visit(cand->first, ret);

            // Encountered a cycle
            if (ret == nullptr) {
                return nullptr;
            }
            cand = std::find_if_not(mark_list.begin(), mark_list.end(), not_perm);
        }
        return ret;
    }


};

int main() {

    std::ifstream file("day5.txt");

    // File failed to open.
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1; 
    }

    bool is_rules = true;
    unsigned number_of_invalid = 0;
    unsigned sum_of_middles = 0;
    int line_number = 0;
    std::vector<std::string> rules;
    for (std::string line; std::getline(file, line);) {

        // Found the separation between rules and sequences
        if (line == "") {
            is_rules = false;
            continue;
        }
        else if (is_rules) {
            rules.push_back(line);
        }

        // This is letting every single update pass.
        // TODO: Figure out why this is happening
        // The fourth update is invalid
        else {
            graph g = graph(rules, line);
            std::deque<int>* ret = g.dfs();

            // There is a cycle
            if (ret == nullptr) {
                continue;
            }

            // Otherwise, check that the update has an improper ordering
            std::vector<int> ret_vec(std::make_move_iterator(ret->begin()),
                                     std::make_move_iterator(ret->end()));
            if (ret_vec == g.get_vals()) {
                continue;
            } 

            number_of_invalid++;
            unsigned middle = (*ret)[ret->size()/2];
            sum_of_middles += middle;
        }

        line_number++;
    }

    std::cout << "Number of Invalid: " << number_of_invalid << std::endl;
    std::cout << "Sum of Middles: " << sum_of_middles << std::endl;

    return 0;
}