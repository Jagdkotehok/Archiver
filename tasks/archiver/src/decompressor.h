//
// Created by Kotehok3 on 21.10.2022.
//

#pragma once

#include "reader.h"
#include "utility.h"

class Decompressor {
public:
    bool DecompressFile(Reader& working_reader);
};