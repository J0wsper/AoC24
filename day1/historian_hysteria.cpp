#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>

// Helper function for quicksort
int partition(std::vector<int>* arr, int low, int high) {
    int pivot = (*arr)[high];
    int i = high;
    int j = low;

    for (int k = low; k < high; k++) {

        // If the element is greater than the pivot, place it to the right
        if ((*arr)[j] >= pivot) {
            const int val = (*arr)[j];
            
            // Making the quicksort in-place
            arr->erase(arr->begin()+j);
            arr->insert(arr->begin()+i, val);
            
            i -= 1;
        }

        // Otherwise, continue
        else {
            j++;
        }
    }
    
    // Return the new position of the pivot
    return i;
}


// Implementing in-place quicksort
void quicksort(std::vector<int>* arr, int low = -1, int high = -1) {
    if (low == -1 || high == -1) {
        low = 0;
        high = arr->size()-1;
    }

    // You have reached the end of the recursion
    if (high <= low) {
        return;
    }

    // Finding the pivot member with the helper function
    int pivot = partition(arr, low, high);

    // Sorting the two subarrays to the left and right of the pivot
    quicksort(arr, low, pivot-1);
    quicksort(arr, pivot+1, high);
}

// Simple binary search that returns how many times it found val in arr
int counting_binary_search(std::vector<int> arr, int val, int low = -1, int high = -1) {
    if (low == -1 || high == -1) {
        low = 0;
        high = arr.size()-1;
    }
    int mid = 0;
    int count = 0;
    while (low <= high) {
        mid = low+(high-low)/2;

        // We found the value
        if (arr[mid] == val) {
            break;
        }

        // Pivoting and resizing our examination space
        else if (arr[mid] < val) {
            low = mid+1;
        }
        else {
            high = mid-1;
        }
    }

    // If we have terminated and not found val, return 0
    if (arr[mid] != val) {
        return 0;
    }

    // Otherwise, count them
    else {
        int curr = mid;
        while (arr[curr] == val) {
            curr--;
        }
        curr++;
        while (arr[curr] == val) {
            curr++;
            count++;
        }
    }

    return count;
}

int main() {

    // Part 1

    std::ifstream file("day1.txt");

    // File failed to open.
    if (!file.is_open()) {
        std::cerr << "File failed to open" << std::endl;
        return -1; 
    }

    // Otherwise, get each line
    std::vector<int> arr1;
    std::vector<int> arr2;
    for (std::string line; std::getline(file, line, '\n');) {

        // Finding the two strings that correspond to the 5-digit values
        const std::string val1_string = line.substr(0, 5);
        const std::string val2_string = line.substr(8);

        // Turning them into integers
        const int val1 = std::stoi(val1_string);
        const int val2 = std::stoi(val2_string);

        // Adding them to our array.
        arr1.push_back(val1);
        arr2.push_back(val2);
    }

    if (arr1.size() != arr2.size()) {
        std::cerr << "Arrays are differing sizes" << std::endl;
        return -1;
    }

    // Sorting the arrays
    quicksort(&arr1);
    quicksort(&arr2);

    // Subtracting the values from each position in the arrays
    int diff = 0;
    for (int i = 0; i < arr1.size(); i++) {
        diff += abs(arr1[i]-arr2[i]);
    }
    std::cout << "Difference: " << diff << std::endl;

    // Part 2

    int similarity_score = 0;
    for (int j = 0; j < arr1.size(); j++) {
        int number_found = counting_binary_search(arr2, arr1[j]);
        similarity_score += number_found*arr1[j];
    }
    std::cout << "Similarity Score: " << similarity_score << std::endl;

    return 0;
}