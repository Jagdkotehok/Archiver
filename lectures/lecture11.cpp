#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <tuple>

template <typename T>
auto Sub(T a, T b) {
    return a - b;
}

template <typename T>
class UniqPtr {
public:
    explicit UniqPtr() = default;
    explicit UniqPtr(T* ptr) : ptr_(ptr) {}

    T& operator*() {
        return *ptr_;
    }

    T* operator->() {
        return ptr_;
    }

    template<typename R>
    R Cast() const {
        return static_cast<R>(*ptr_);
    }

    ~UniqPtr() {
        delete ptr_;
    }

private:
    T* ptr_ = nullptr;
};

template <typename T>
class UniqPtr<T[]> {
public:
    explicit UniqPtr() = default;
    explicit UniqPtr(T* ptr) : ptr_(ptr) {}

    T& operator[](size_t index) {
        return ptr_[index];
    }

    ~UniqPtr() {
        delete[] ptr_;
    }

private:
    T* ptr_ = nullptr;
};

template <typename T, typename... Args>
UniqPtr<T> MakeUniq(Args... args) {
    return UniqPtr<T>(new T(args...));
}

using IntVector = std::vector<int>;

int main() {
    auto ptr = MakeUniq<std::vector<int>>(100);
    auto ptr2 = MakeUniq<std::tuple<int, double, std::string>>(100, 9.9, "hello");

    std::cout << ptr->size() << std::endl;

    return 0;
}
