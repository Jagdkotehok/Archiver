#include <iostream>
#include <vector>

class Vector {
public:
    Vector() {
    }

    explicit Vector(size_t size) : values_(size) {
    }

    Vector(const Vector& vector) : values_(vector.values_) {
        std::cout << "COPY" << std::endl;
    }

    Vector& operator=(const Vector& vector) {
        if (this == &vector) {
            return *this;
        }

        values_ = vector.values_;
        return *this;
    }

    ~Vector() {
    }

    size_t GetSize() const {
        return this->values_.size();
    }

    double& operator[](size_t index) {
        return values_[index];
    }

    const double& operator[](size_t index) const {
        return values_[index];
    }

    auto operator<=>(const Vector& other) const = default;

    Vector operator+(int value) const {
        Vector result = *this;
        for (double& v : result.values_) {
            v += value;
        }
        return result;
    }

    friend Vector operator+(int value, const Vector& vector) {
        Vector result = vector;
        for (double& v : result.values_) {
            v += value;
        }
        return result;
    }

    Vector& operator++() {
        for (double& v : values_) {
            ++v;
        }
        return *this;
    }

    Vector operator++(int) { // postfix
        Vector result = *this;
        ++*this;
        return result;
    }

    explicit operator bool() const {
        return !values_.empty();
    }

private:
    std::vector<double> values_;
};

std::ostream& operator<<(std::ostream& stream, const Vector& vector) {
    stream << "Vector of size " << vector.GetSize();
    return stream;
}

int main() {
    std::cout << std::boolalpha;

    Vector vector1(10);
    Vector vector2;

    vector2 = vector1 + 100;
    vector2 = 100 + vector1;
    vector2++;

    bool b = static_cast<bool>(vector1);

    if (vector1) {
        std::cout << "Not empty" << std::endl;
    }

    std::cout << vector2 << std::endl;

    return 0;
}
