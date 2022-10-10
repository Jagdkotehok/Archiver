#include "poly.h"

Poly::Poly() {
    coefficients_.clear();
}

Poly::Poly(std::initializer_list<int64_t> list) {
    int64_t cur_power = 0;
    for (const int64_t& y : list) {
        if (y != 0) {
            coefficients_[cur_power++] = y;
        } else {
            ++cur_power;
        }
    }
}

Poly::Poly(std::initializer_list<std::pair<int64_t, int64_t>> list) {
    for (const auto& [x, y] : list) {
        if (y != 0) {
            coefficients_[x] = y;
        }
    }
}

int64_t Poly::operator()(int64_t x) const {
    int64_t result = 0;
    for (const auto& [power, coefficient] : coefficients_) {
        result += coefficient * BinPow(x, power);
    }
    return result;
}

bool Poly::operator==(const Poly& other) const {
    if (coefficients_.size() != other.coefficients_.size()) {
        return false;
    }
    for (const auto& [x, y] : coefficients_) {
        if (other.coefficients_.find(x) == other.coefficients_.end()) {
            return false;
        } else if (other.coefficients_.at(x) != y) {
            return false;
        }
    }
    return true;
}

bool Poly::operator!=(const Poly& other) const {
    return !(*this == other);
}
Poly& Poly::operator+=(const Poly& other) {
    for (const auto& [x, y] : other.coefficients_) {
        coefficients_[x] += y;
        if (coefficients_[x] == 0) {
            coefficients_.erase(x);
        }
    }
    return *this;
}

Poly& Poly::operator-=(const Poly& other) {
    for (const auto& [x, y] : other.coefficients_) {
        coefficients_[x] -= y;
        if (coefficients_[x] == 0) {
            coefficients_.erase(x);
        }
    }
    return *this;
}

Poly& Poly::operator*=(const Poly& other) {
    Poly initial = Poly();
    initial.coefficients_ = coefficients_;
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

Poly Poly::operator+(const Poly& other) {
    Poly result = Poly();
    for (const auto& [x, y] : coefficients_) {
        result.coefficients_[x] += y;
        if (result.coefficients_[x] == 0) {
            result.coefficients_.erase(x);
        }
    }
    for (const auto& [x, y] : other.coefficients_) {
        result.coefficients_[x] += y;
        if (result.coefficients_[x] == 0) {
            result.coefficients_.erase(x);
        }
    }
    return result;
}

Poly Poly::operator-(const Poly& other) {
    Poly result = Poly();
    for (const auto& [x, y] : coefficients_) {
        result.coefficients_[x] += y;
        if (result.coefficients_[x] == 0) {
            result.coefficients_.erase(x);
        }
    }
    for (const auto& [x, y] : other.coefficients_) {
        result.coefficients_[x] -= y;
        if (result.coefficients_[x] == 0) {
            result.coefficients_.erase(x);
        }
    }
    return result;
}

Poly Poly::operator*(const Poly& other) {
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

Poly& Poly::operator=(const Poly& other) {
    coefficients_.clear();
    for (const auto& [x, y] : other.coefficients_) {
        coefficients_[x] = y;
        if (coefficients_[x] == 0) {
            coefficients_.erase(y);
        }
    }
    return *this;
}

Poly Poly::operator-() const {
    Poly result = Poly();
    for (const auto& [x, y] : coefficients_) {
        result.coefficients_[x] = -y;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Poly& poly) {
    os << "y = ";
    if (poly.coefficients_.empty()) {
        os << 0;
    } else {
        bool is_first_coefficient = true;
        for (const auto& [x, y] : poly.coefficients_) {
            if (is_first_coefficient) {
                if (y < 0) {
                    os << "-";
                }
            } else {
                if (y < 0) {
                    os << " - ";
                } else {
                    os << " + ";
                }
            }
            if (x == 0) {
                os << std::abs(y);
            } else if (x == 1) {
                os << std::abs(y) << "x";
            } else {
                os << std::abs(y) << "x^" << x;
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
