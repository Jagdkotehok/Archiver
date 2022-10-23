//
// Created by Kotehok3 on 20.10.2022.
//

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "trie.h"
#include "writer.h"


class Compressor {
public:
    std::unordered_map<utility::Symbol, uint64_t> GetSymbolFrequencies(const std::string& filename);

    std::vector<std::pair<utility::Symbol, utility::Code>> GetAnyHuffmanCode(
        std::unordered_map<utility::Symbol, uint64_t>& symbol_frequencies);

    void GetCanonicalHuffmanCode(
        std::vector<std::pair<utility::Symbol, utility::Code>>& huffman_code);

    void CompressFile(const std::string& filename, Writer& current_writer, bool is_last_file);
};
