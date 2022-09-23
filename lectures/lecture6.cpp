#include <iostream>
#include <vector>

class Vector {
public:
    Vector() {

    }

    explicit Vector(size_t size = 0) :  {
        std::cout << GetSize() << std::endl;
    }

    Vector(size_t half_size, size_t mult) : Vector(half_size * mult) {
        ////
    }

    ~Vector() {
        std::cout << "Destructor" << std::endl;
    }

    size_t GetSize() const {
        return this->values_.size();
    }

    double& At(size_t index) {
        // Vector* const this
        std::cout << "non-const" << std::endl;
        return values_[index];
    }

    const double& At(size_t index) const {
        std::cout << "const" << std::endl;
        return values_[index];
    }

    double CalcNorm2() const {
        double sum = 0;
        for (double v : values_) {
            sum += v*v;
        }

        return sum;
    }

private:
    std::vector<double> values_;
};

void Print(const Vector& vector) {
    std::cout << vector.At(0) << std::endl;
    std::cout << vector.GetSize() << std::endl;
}

void Print2(Vector vector) {
    std::cout << vector.GetSize() << std::endl;
}

int main() {
    Vector vector(10);

    Print2(100);

    std::cout << vector.GetSize() << std::endl;

    return 0;
}

