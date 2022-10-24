//
// Created by Kotehok3 on 22.10.2022.
//

#pragma once

#include <exception>

class CustomException : public std::exception {
private:
    const char* message_;

public:
    explicit CustomException(const char* msg);

    const char* what() const noexcept override;
};