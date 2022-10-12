#pragma once

#include <map>
#include <cstdint>
#include <ostream>

class Poly {
public:
    std::map<int64_t, int64_t, std::greater<int64_t>> coefficients_;

    void NormalizeCoefficients();

    explicit Poly();
    Poly(const std::initializer_list<int64_t>& list);
    Poly(const std::initializer_list<std::pair<int64_t, int64_t>>& list);

    int64_t operator()(int64_t x) const;

    bool operator==(const Poly& other) const;
    bool operator!=(const Poly& other) const;

    Poly& operator+=(const Poly& other);
    Poly& operator-=(const Poly& other);
    Poly& operator*=(const Poly& other);

    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;
    Poly operator*(const Poly& other) const;

    Poly operator-() const;

    friend std::ostream& operator<<(std::ostream& os, const Poly& poly);

    int64_t BinPow(int64_t x, int64_t power) const;
};
