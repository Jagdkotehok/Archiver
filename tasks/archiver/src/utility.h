//
// Created by Kotehok3 on 20.10.2022.
//

#pragma once

#include <cstdint>
#include <vector>
#include <limits>

namespace utility {
using Code = std::vector<bool>;

using Symbol = uint16_t;

const uint16_t MAX_SYMBOL = std::numeric_limits<uint16_t>::max();
const uint16_t FILENAME_END = 256;
const uint16_t ONE_MORE_FILE = 257;
const uint16_t ARCHIVE_END = 258;
}  // namespace utility