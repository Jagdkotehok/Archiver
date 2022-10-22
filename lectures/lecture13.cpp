#include <iostream>
#include <vector>
#include <functional>

using Operation = int(int, int);

std::vector<int> ZipMap(const std::vector<int>& a, const std::vector<int>& b, std::function<Operation> op) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("a.size != b.size");
    }

    std::vector<int> result;

    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back(op(a[i], b[i]));
    }

    return result;
}

struct Foo {
    int Mult(int a, int b) {
        return a * b + x;
    }

    int Do() {
        return [=](int a, int b) { return this->Mult(a, b); }(1, 2);
    }

    int x = 100;
};

int Sub(int a, int b) {
    return a - b;
}

void Print(const std::vector<int>& vector) {
    for (const auto& v: vector) {
        std::cout << v << " ";
    }
}

using namespace std::placeholders;

class Lambda {
public:
    Lambda(int& x) : x(x) {}

    template<typename T1, typename T2>
    auto operator()(T1 a, T2 b) const {
        return a * b * x;
    }

private:
    int& x;
};

int main() {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {10, 20, 30};

    int (Foo::*method)(int, int) = &Foo::Mult;

    Foo foo;

    auto func1 = std::mem_fn(&Foo::Mult);
    std::cout << func1(foo, 1, 2) << std::endl;

    auto func2 = std::bind(&Foo::Mult, &foo, _1, _2);

    std::function<Operation> func3;

    int x = 1000;
    auto func4 = [=](auto a, auto b) { return a * b; };
    std::cout << func4(1, 2) << std::endl;
    std::cout << func4(1, 2.0) << std::endl;
    std::cout << func4(1.0, 2.0) << std::endl;

    Print(ZipMap(a, b, func4));

    return 0;
}

