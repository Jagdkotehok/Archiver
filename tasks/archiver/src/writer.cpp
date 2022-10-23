//
// Created by Kotehok3 on 21.10.2022.
//
#include "writer.h"

Writer::Writer(const std::string& filename) {
    working_stream_.open(filename, std::ios::out | std::ios::binary);
}

Writer::~Writer() {
    if (need_to_write) {
        working_stream_.put(static_cast<char>(char_to_write));
    }
    working_stream_.close();
}

void Writer::Write(utility::Symbol symbol) {
    for (uint8_t i = 0; i < 9; ++i) {
        if ((1 << (8 - i)) & symbol) {
            WriteOne();
        } else {
            WriteZero();
        }
    }
}

void Writer::Write(utility::Code code) {
    for (const bool value : code) {
        if (value) {
            WriteOne();
        } else {
            WriteZero();
        }
    }
}

void Writer::WriteOne() {
    char_to_write |= (1 << (7 - working_power));
    ++working_power;
    need_to_write = true;
    if (working_power == 8) {
        working_stream_.put(static_cast<char>(char_to_write));
        working_power = 0;
        char_to_write = 0;
        need_to_write = false;
    }
}

void Writer::WriteZero() {
    ++working_power;
    need_to_write = true;
    if (working_power == 8) {
        working_stream_.put(static_cast<char>(char_to_write));
        working_power = 0;
        char_to_write = 0;
        need_to_write = false;
    }
}
