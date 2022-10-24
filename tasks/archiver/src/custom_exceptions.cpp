//
// Created by Kotehok3 on 22.10.2022.
//
#include "custom_exceptions.h"

CustomException::CustomException(const char* msg) : message_(msg) {
}

const char* CustomException::what() const noexcept {
    return message_;
}
