//
// Created by Kotehok3 on 21.10.2022.
//

#include "decompressor.h"
#include "trie.h"

bool Decompressor::DecompressFile(Reader& working_reader) {
    utility::Symbol symbols_count = working_reader.ReadSymbol();

    std::vector<std::pair<utility::Symbol, utility::Code>> canonical_huffman_code(symbols_count);
    for (auto& [symbol, code] : canonical_huffman_code) {
        symbol = working_reader.ReadSymbol();
    }

    uint16_t current_symbol_count = 0;
    std::vector<uint16_t> length_count;

    while (current_symbol_count < symbols_count) {
        uint16_t current_count = working_reader.ReadSymbol();
        current_symbol_count += current_count;
        length_count.emplace_back(current_count);
    }

    utility::Code current_code;
    size_t needed_length = 0;

    for (auto& [symbol, code] : canonical_huffman_code) {
        while (length_count[needed_length] == 0) {
            ++needed_length;
        }

        --length_count[needed_length];

        if (current_code.empty()) {
            current_code.assign(needed_length, false);
        } else {
            /// adding 1 to the code
            for (ssize_t i = static_cast<ssize_t>(current_code.size()) - 1; i > -1; --i) {
                if (current_code[i]) {
                    current_code[i] = false;
                } else {
                    current_code[i] = true;
                    break;
                }
            }
        }

        while (current_code.size() <= needed_length) {
            current_code.emplace_back(0);
        }

        code = current_code;
    }

    Trie trie(new Trie::TrieVertex());
    for (const auto& [symbol, code] : canonical_huffman_code) {
        trie.AddSymbol(trie.root_, code, 0, symbol);
    }

    std::string resulting_filename;
    bool is_reading_file_content = false;  /// 0 -- reading filename, 1 -- reading file content

    std::ofstream output_file;

    while (true) {
        Trie::TrieVertex* v = trie.root_;
        while (v->left_child_ != nullptr || v->right_child_ != nullptr) {
            bool bit = working_reader.ReadBit();
            if (bit) {
                v = v->right_child_;
            } else {
                v = v->left_child_;
            }
        }

        utility::Symbol symbol = v->value_;

        if (symbol == utility::FILENAME_END) {
            is_reading_file_content = true;
            output_file.open(resulting_filename, std::ios::binary);
        } else if (symbol == utility::ARCHIVE_END) {
            output_file.close();
            return false;
        } else if (symbol == utility::ONE_MORE_FILE) {
            output_file.close();
            return true;
        } else {
            if (!is_reading_file_content) {
                resulting_filename += static_cast<signed char>(symbol);
            } else {
                output_file << static_cast<signed char>(symbol);
            }
        }
    }
}
