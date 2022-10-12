#include "poly.h"

Polynom NormalizeCoefficients(Polynom polynom) {
    std::sort(polynom.rbegin(), polynom.rend());
    Polynom result;
    int64_t coefficient = 0;
    for (size_t i = 0; i < polynom.size(); ++i) {
        if (i != 0 && polynom[i].first != polynom[i - 1].first && coefficient != 0) {
            result.emplace_back(polynom[i - 1].first, coefficient);
            coefficient = 0;
        }
        coefficient += polynom[i].second;
    }
    if (coefficient && !polynom.empty()) {
        result.emplace_back(polynom.back().first, coefficient);
    }
    return result;
}

Poly::Poly() : poly_() {
}

Poly::Poly(const std::initializer_list<int64_t>& list) {
    size_t current_power = 0;
    poly_.reserve(list.size());
    for (const auto& coefficient : list) {
        poly_.emplace_back(current_power++, coefficient);
    }
    poly_ = NormalizeCoefficients(poly_);
}

Poly::Poly(const std::initializer_list<std::pair<int64_t, int64_t>>& list) {
    poly_.reserve(list.size());
    for (const auto& [power, coefficient] : list) {
        poly_.emplace_back(power, coefficient);
    }
    poly_ = NormalizeCoefficients(poly_);
}

int64_t Poly::operator()(const int64_t val) const {
    int64_t result = 0;
    for (const auto& [power, coefficient] : poly_) {
        result += coefficient * BinPow(val, power);
    }
    return result;
}

bool Poly::operator==(const Poly& other) const {
    return poly_ == other.poly_;
}

bool Poly::operator!=(const Poly& other) const {
    return !(*this == other);
}

Poly& Poly::operator+=(const Poly& other) {
    poly_.reserve(poly_.size() + other.poly_.size());
    for (const auto& [power, coefficient] : other.poly_) {
        poly_.emplace_back(power, coefficient);
    }
    poly_ = NormalizeCoefficients(poly_);
    return *this;
}

Poly& Poly::operator-=(const Poly& other) {
    poly_.reserve(poly_.size() + other.poly_.size());
    for (const auto& [power, coefficient] : other.poly_) {
        poly_.emplace_back(power, -coefficient);
    }
    poly_ = NormalizeCoefficients(poly_);
    return *this;
}

Poly& Poly::operator*=(const Poly& other) {
    Poly result;
    result.poly_.reserve(poly_.size() * other.poly_.size());
    for (const auto& [power1, coefficient1] : poly_) {
        for (const auto& [power2, coefficient2] : other.poly_) {
            result.poly_.emplace_back(power1 + power2, coefficient1 + coefficient2);
        }
    }
    result.poly_ = NormalizeCoefficients(result.poly_);
    return *this = result;
}

Poly Poly::operator+(const Poly& other) const {
    Poly result = *this;
    result.poly_.reserve(poly_.size() + other.poly_.size());
    for (const auto& [power, coefficient] : other.poly_) {
        result.poly_.emplace_back(power, coefficient);
    }
    result.poly_ = NormalizeCoefficients(result.poly_);
    return result;
}

Poly Poly::operator-(const Poly& other) const {
    Poly result = *this;
    result.poly_.reserve(poly_.size() + other.poly_.size());
    for (const auto& [power, coefficient] : other.poly_) {
        result.poly_.emplace_back(power, -coefficient);
    }
    result.poly_ = NormalizeCoefficients(result.poly_);
    return result;
}

Poly Poly::operator*(const Poly& other) const {
    Poly result;
    result.poly_.reserve(poly_.size() * other.poly_.size());
    for (const auto& [power1, coefficient1] : poly_) {
        for (const auto& [power2, coefficient2] : other.poly_) {
            result.poly_.emplace_back(power1 + power2, coefficient1 * coefficient2);
        }
    }
    result.poly_ = NormalizeCoefficients(result.poly_);
    return result;
}

Poly Poly::operator-() const {
    Poly result = *this;
    for (auto& [power, coefficient] : result.poly_) {
        coefficient *= -1;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    os << "y = ";
    if (poly.poly_.empty()) {
        os << 0;
    } else {
        bool is_first_coefficient = true;
        for (const auto& [power, coefficient] : poly.poly_) {
            if (is_first_coefficient) {
                if (coefficient < 0) {
                    os << "-";
                }
            } else {
                if (coefficient < 0) {
                    os << " - ";
                } else {
                    os << " + ";
                }
            }
            if (power == 0) {
                os << std::abs(coefficient);
            } else {
                os << std::abs(coefficient) << "x^" << power;
            }
            is_first_coefficient = false;
        }
    }
    return os;
}
int64_t Poly::BinPow(int64_t val, int64_t power) const {
    if (power == 0) {
        return 1;
    } else if (power % 2 == 1) {
        return val * BinPow(val, power - 1);
    } else {
        return BinPow(val * val, power / 2);
    }
}
