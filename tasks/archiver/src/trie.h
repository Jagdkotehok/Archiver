//
// Created by Kotehok3 on 20.10.2022.
//

#pragma once

#include <cstdint>
#include <vector>

#include "utility.h"

class Trie {
public:
    class TrieVertex {
    public:
        TrieVertex* left_child_;
        TrieVertex* right_child_;

        utility::Symbol value_;

        explicit TrieVertex();
        TrieVertex(TrieVertex* left, TrieVertex* right, utility::Symbol value);

        ~TrieVertex();
    };

    TrieVertex* root_;

    explicit Trie();
    explicit Trie(TrieVertex* root);
    ~Trie();

    void GetHuffmanCode(TrieVertex* v, std::vector<std::pair<utility::Symbol, utility::Code>>& result,
                        utility::Code& current_code);
    void AddSymbol(TrieVertex* v, const utility::Code& code, size_t id, utility::Symbol symbol);
};