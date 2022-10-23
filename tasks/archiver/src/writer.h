//
// Created by Kotehok3 on 21.10.2022.
//

#pragma once

#include <fstream>
#include <cstdint>

#include "utility.h"

class Writer {
public:
    std::ofstream working_stream_;

    uint8_t char_to_write = 0;
    uint8_t working_power = 0;
    bool need_to_write = false;

    explicit Writer(const std::string& filename);
    ~Writer();

    void Write(utility::Code code); /// random count of bits value
    void Write(utility::Symbol symbol); /// strict 9-bit value

private:
    void WriteOne();
    void WriteZero();
};