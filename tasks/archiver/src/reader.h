//
// Created by Kotehok3 on 21.10.2022.
//

#pragma once

#include <fstream>

#include "utility.h"

class Reader {
public:
    std::ifstream working_stream_;

    uint8_t char_to_read = 0;
    uint8_t working_power = 0;

    explicit Reader(const std::string& filename);
    ~Reader();

    uint8_t ReadBit();
    utility::Symbol ReadSymbol();
};