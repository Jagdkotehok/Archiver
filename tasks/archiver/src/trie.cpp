//
// Created by Kotehok3 on 20.10.2022.
//

#include "trie.h"

Trie::TrieVertex::TrieVertex() {
    left_child_ = nullptr;
    right_child_ = nullptr;
    value_ = utility::MAX_SYMBOL;
}

Trie::TrieVertex::~TrieVertex() {
    delete left_child_;
    delete right_child_;
}

Trie::TrieVertex::TrieVertex(Trie::TrieVertex *left, Trie::TrieVertex *right, utility::Symbol value)
    : left_child_(left), right_child_(right), value_(value) {
}

Trie::Trie() {
    root_ = nullptr;
}

Trie::~Trie() {
    delete root_;
}
void Trie::GetHuffmanCode(Trie::TrieVertex *v, std::vector<std::pair<utility::Symbol, utility::Code>> &result,
                          utility::Code &current_code) {
    if (v->left_child_ == nullptr && v->right_child_ == nullptr) {
        result.emplace_back(v->value_, current_code);
    }
    if (v->left_child_ != nullptr) {
        current_code.emplace_back(false);
        GetHuffmanCode(v->left_child_, result, current_code);
        current_code.pop_back();
    }
    if (v->right_child_ != nullptr) {
        current_code.emplace_back(true);
        GetHuffmanCode(v->right_child_, result, current_code);
        current_code.pop_back();
    }
}
Trie::Trie(Trie::TrieVertex *root) : root_(root) {

}

void Trie::AddSymbol(Trie::TrieVertex *v, const utility::Code &code, size_t id, utility::Symbol symbol) {
    if (id == code.size()) {
        v->value_ = symbol;
        return;
    }
    if (code[id]) {
        if (v->right_child_ == nullptr) {
            v->right_child_ = new TrieVertex();
        }
        AddSymbol(v->right_child_, code, id + 1, symbol);
    } else {
        if (v->left_child_ == nullptr) {
            v->left_child_ = new TrieVertex();
        }
        AddSymbol(v->left_child_, code, id + 1, symbol);
    }
}
