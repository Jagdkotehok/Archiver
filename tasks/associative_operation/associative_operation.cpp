#include "associative_operation.h"

bool IsAssociative(const std::vector<std::vector<size_t>>& table) {
    size_t n = table.size();
    for (size_t x = 0; x < n; ++x) {
        for (size_t y = 0; y < n; ++y) {
            for (size_t z = 0; z < n; ++z) {
                size_t value_1 = table[table[x][y]][z];
                size_t value_2 = table[x][table[y][z]];
                if (value_1 != value_2) {
                    return false;
                }
            }
        }
    }
    return true;
}
