#include "palindrome.h"

const int SPACE_CODE = static_cast<int>(' ');

bool IsPalindrome(const std::string& str) {
    if (str.empty()) {
        return true;
    }
    size_t left_index = 0;
    size_t right_index = str.size() - 1;
    while (left_index < right_index) {
        int left_symbol = static_cast<int>(str[left_index]);
        int right_symbol = static_cast<int>(str[right_index]);
        if (left_symbol == SPACE_CODE) {
            ++left_index;
            continue;
        }
        if (right_symbol == SPACE_CODE) {
            --right_index;
            continue;
        }
        if (left_symbol != right_symbol) {
            return false;
        }
        ++left_index;
        --right_index;
    }
    return true;
}
