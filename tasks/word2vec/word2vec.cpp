#include "word2vec.h"

#include <vector>

std::vector<std::string> FindClosestWords(const std::vector<std::string>& words,
                                          const std::vector<std::vector<int>>& vectors) {
    int max_word_value = -INT_MAX;
    for (size_t i = 1; i < vectors.size(); ++i) {
        int current_word_value = 0;
        for (size_t j = 0; j < vectors[i].size(); ++j) {
            current_word_value += vectors[0][j] * vectors[i][j];
        }
        max_word_value = std::max(max_word_value, current_word_value);
    }
    std::vector<std::string> closest_words;
    for (size_t i = 1; i < vectors.size(); ++i) {
        int current_word_value = 0;
        for (size_t j = 0; j < vectors[i].size(); ++j) {
            current_word_value += vectors[0][j] * vectors[i][j];
        }
        if (current_word_value == max_word_value) {
            closest_words.emplace_back(words[i]);
        }
    }
    return closest_words;
}
