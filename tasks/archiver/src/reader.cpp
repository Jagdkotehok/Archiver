//
// Created by Kotehok3 on 21.10.2022.
//
#include "reader.h"
#include "custom_exceptions.h"

Reader::Reader(const std::string& filename) {
    working_stream_.open(filename, std::ios::in | std::ios::binary);
    working_power = 0;
    if (working_stream_.peek() != EOF) {
        char_to_read = working_stream_.get();
        is_char_valid = true;
    } else {
        is_char_valid = false;
    }
}

Reader::~Reader() {
    working_stream_.close();
}
uint8_t Reader::ReadBit() {
    if (!is_char_valid) {
        throw CustomException("Reached end of reading stream");
    }
    uint8_t result = 0;
    if (char_to_read & (1 << (7 - working_power))) {
        result = 1;
    }
    ++working_power;
    if (working_power == 8) {
        working_power = 0;
        if (working_stream_.peek() != EOF) {
            char_to_read = working_stream_.get();
            is_char_valid = true;
        } else {
            is_char_valid = false;
        }
    }
    return result;
}
utility::Symbol Reader::ReadSymbol() {
    utility::Symbol result = 0;
    for (uint8_t i = 0; i < 9; ++i) {
        if (ReadBit()) {
            result |= (1 << (8 - i));
        }
    }
    return result;
}
