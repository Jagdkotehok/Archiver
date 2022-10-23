//
// Created by Kotehok3 on 20.10.2022.
//

#include <set>
#include <unordered_map>
#include <string>

#include "compressor.h"

std::unordered_map<utility::Symbol, uint64_t> Compressor::GetSymbolFrequencies(const std::string& filename) {
    std::unordered_map<utility::Symbol, uint64_t> symbol_frequencies;

    for (const uint8_t cur_char : filename) {
        ++symbol_frequencies[cur_char];
    }

    ++symbol_frequencies[utility::ARCHIVE_END];
    ++symbol_frequencies[utility::ONE_MORE_FILE];
    ++symbol_frequencies[utility::FILENAME_END];

    std::ifstream file_stream(filename, std::ios::out | std::ios::binary);

    while (file_stream.peek() != EOF) {
        auto c = file_stream.get();
        ++symbol_frequencies[static_cast<uint8_t>(c)];
    }

    return symbol_frequencies;
}

std::vector<std::pair<utility::Symbol, utility::Code>> Compressor::GetAnyHuffmanCode(
    std::unordered_map<utility::Symbol, uint64_t>& symbol_frequencies) {

    auto comp = [](const std::pair<uint64_t, Trie::TrieVertex*>& left,
                   const std::pair<uint64_t, Trie::TrieVertex*>& right) {
        if (left.first != right.first) {
            return left.first < right.first;  /// ordering by priority increasing
        } else {
            return left.second->value_ < right.second->value_;  /// ordering by minimal vertex in subtree
        }
    };

    std::set<std::pair<uint64_t, Trie::TrieVertex*>, decltype(comp)> p_queue(comp);

    for (auto& [symbol, frequency] : symbol_frequencies) {
        Trie::TrieVertex* cur_vertex = new Trie::TrieVertex(nullptr, nullptr, symbol);
        p_queue.emplace(frequency, cur_vertex);
    }

    while (p_queue.size() > 1) {
        auto [first_vertex_priority, first_vertex_pointer] = *p_queue.begin();
        p_queue.erase(p_queue.begin());
        auto [second_vertex_priority, second_vertex_pointer] = *p_queue.begin();
        p_queue.erase(p_queue.begin());
        Trie::TrieVertex* new_vertex =
            new Trie::TrieVertex(first_vertex_pointer, second_vertex_pointer,
                                 std::min(first_vertex_pointer->value_, second_vertex_pointer->value_));
        p_queue.emplace(first_vertex_priority + second_vertex_priority, new_vertex);
    }

    Trie cur_trie = Trie(p_queue.begin()->second);

    utility::Code temp_code = {};
    std::vector<std::pair<utility::Symbol, utility::Code>> result = {};

    cur_trie.GetHuffmanCode(cur_trie.root_, result, temp_code);

    return result;
}

void Compressor::GetCanonicalHuffmanCode(std::vector<std::pair<utility::Symbol, utility::Code>>& huffman_code) {

    std::sort(huffman_code.begin(), huffman_code.end(),
              [](const std::pair<utility::Symbol, utility::Code>& left,
                 const std::pair<utility::Symbol, utility::Code>& right) {
                  if (left.second.size() != right.second.size()) {
                      return left.second.size() < right.second.size();  /// ordering by size of code
                  } else {
                      return left.first < right.first;  /// then ordering by symbol
                  }
              });

    utility::Code current_code = {};

    for (auto& [symbol, code] : huffman_code) {

        if (current_code.empty()) {
            current_code.assign(code.size(), false);
        } else {
            /// adding 1 to current_code
            for (ssize_t pos = static_cast<ssize_t>(current_code.size()) - 1; pos > -1; --pos) {
                if (current_code[pos]) {
                    current_code[pos] = false;
                } else {
                    current_code[pos] = true;
                    break;
                }
            }
        }
        while (current_code.size() < code.size()) {
            current_code.emplace_back(false);
        }
        code = current_code;
    }
}

void Compressor::CompressFile(const std::string& filename, Writer& current_writer, bool is_last_file) {

    auto frequencies = GetSymbolFrequencies(filename);
    auto huffman_code = GetAnyHuffmanCode(frequencies);
    GetCanonicalHuffmanCode(huffman_code);

    std::vector<uint16_t> length_count;
    std::unordered_map<utility::Symbol, utility::Code> symbol_to_code;

    for (const auto& [symbol, code] : huffman_code) {
        symbol_to_code[symbol] = code;

        while (length_count.size() <= code.size()) {
            length_count.emplace_back(0);
        }

        ++length_count[code.size()];
    }

    current_writer.Write(huffman_code.size());  /// writing count of distinct symbols

    for (const auto& [symbol, code] : huffman_code) {
        current_writer.Write(symbol);
    }  /// writing every distinct symbol in huffman_code order

    for (size_t i = 1; i < length_count.size(); ++i) {
        current_writer.Write(length_count[i]);
    }  /// writing count of code of every length

    for (const uint8_t cur_char : filename) {
        current_writer.Write(symbol_to_code[cur_char]);
    }  /// writing filename

    current_writer.Write(symbol_to_code[utility::FILENAME_END]);  /// writing special symbol

    std::ifstream current_file(filename, std::ios::out | std::ios::binary);

    while (current_file.peek() != EOF) {
        uint8_t cur_symbol = current_file.get();
        current_writer.Write(symbol_to_code[cur_symbol]);
    }  /// writing content of file

    if (is_last_file) {
        current_writer.Write(symbol_to_code[utility::ARCHIVE_END]);
    } else {
        current_writer.Write(symbol_to_code[utility::ONE_MORE_FILE]);
    }  /// writing end of file special symbol
}
