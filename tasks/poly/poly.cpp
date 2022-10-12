#include "poly.h"

Poly::Poly() {
    coefficients_.clear();
}

Poly::Poly(std::initializer_list<int64_t> list) {
    int64_t current_power = 0;
    for (const auto& coefficient : list) {
        if (coefficient != 0) {
            coefficients_[current_power] = coefficient;
        }
        ++current_power;
    }
}

Poly::Poly(std::initializer_list<std::pair<int64_t, int64_t>> list) {
    for (const auto& [power, coefficient] : list) {
        if (coefficient != 0) {
            coefficients_[power] = coefficient;
        }
    }
}

int64_t Poly::operator()(const int64_t x) const {
    int64_t result = 0;
    for (const auto& [power, coefficient] : coefficients_) {
        result += coefficient * BinPow(x, power);
    }
    return result;
}

bool Poly::operator==(const Poly& other) const {
    return coefficients_ == other.coefficients_;
}

bool Poly::operator!=(const Poly& other) const {
    return !(*this == other);
}

Poly& Poly::operator+=(const Poly& other) {
    for (const auto& [power, coefficient] : other.coefficients_) {
        coefficients_[power] += coefficient;
        if (coefficients_[power] == 0) {
            coefficients_.erase(power);
        }
    }
    return *this;
}

Poly& Poly::operator-=(const Poly& other) {
    for (const auto& [power, coefficient] : other.coefficients_) {
        coefficients_[power] -= coefficient;
        if (coefficients_[power] == 0) {
            coefficients_.erase(power);
        }
    }
    return *this;
}

Poly& Poly::operator*=(const Poly& other) {
    Poly initial = *this;
    coefficients_.clear();
    for (const auto& [power1, coefficient1] : initial.coefficients_) {
        for (const auto& [power2, coefficient2] : other.coefficients_) {
            coefficients_[power1 + power2] += coefficient1 * coefficient2;
            if (coefficients_[power1 + power2] == 0) {
                coefficients_.erase(power1 + power2);
            }
        }
    }
    return *this;
}

Poly Poly::operator+(const Poly& other) const {
    Poly result = *this;
    for (const auto& [power, coefficient] : other.coefficients_) {
        result.coefficients_[power] += coefficient;
        if (result.coefficients_[power] == 0) {
            result.coefficients_.erase(power);
        }
    }
    return result;
}

Poly Poly::operator-(const Poly& other) const {
    Poly result = *this;
    for (const auto& [power, coefficient] : other.coefficients_) {
        result.coefficients_[power] -= coefficient;
        if (result.coefficients_[power] == 0) {
            result.coefficients_.erase(power);
        }
    }
    return result;
}

Poly Poly::operator*(const Poly& other) const {
    Poly result = Poly();
    for (const auto& [power1, coefficient1] : coefficients_) {
        for (const auto& [power2, coefficient2] : other.coefficients_) {
            result.coefficients_[power1 + power2] += coefficient1 * coefficient2;
            if (result.coefficients_[power1 + power2] == 0) {
                result.coefficients_.erase(power1 + power2);
            }
        }
    }
    return result;
}

Poly Poly::operator-() const {
    Poly result = *this;
    for (auto& [power, coefficient] : result.coefficients_) {
        coefficient *= -1;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    os << "y = ";
    if (poly.coefficients_.empty()) {
        os << 0;
    } else {
        bool is_first_coefficient = true;
        for (const auto& [power, coefficient] : poly.coefficients_) {
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
int64_t Poly::BinPow(int64_t x, int64_t power) const {
    if (power == 0) {
        return 1;
    } else if (power % 2 == 1) {
        return x * BinPow(x, power - 1);
    } else {
        return BinPow(x * x, power / 2);
    }
}
