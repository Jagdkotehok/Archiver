#include <iostream>
#include <cstring>

class Foo {
public:
    Foo(std::string str) : str_(std::move(str)) {
    }

private:
    std::string str_;
};


class String {
public:
    String() {
        std::cout << "String()" << std::endl;
    }

    String(const char* str) {
        std::cout << "String(const char*)" << std::endl;

        size_ = std::strlen(str);
        data_ = new char[size_];
        std::memcpy(data_, str, size_);
    }

    String(const String& other) {
        std::cout << "String(const String&)"<< std::endl;

        data_ = new char[other.size_];
        size_ = other.size_;
        std::memcpy(data_, other.data_, size_);
    }

    String(String&& other) {
        std::cout << "String(String&&)"<< std::endl;

        data_ = other.data_;
        size_ = other.size_;

        other.data_ = nullptr;
        other.size_ = 0;
    }

    String& operator=(String other) {
        std::cout << "op=(String&&)" << std::endl;

        std::swap(data_, other.data_);
        std::swap(size_, other.size_);

        return *this;
    }

    String operator+(const String& other) const {
        String result;

        result.size_ = size_ + other.size_;
        result.data_ = new char[result.size_];

        std::memcpy(result.data_, data_, size_);
        std::memcpy(result.data_ + size_, other.data_, other.size_);

        return result;
    }

    ~String() {
        std::cout << "~String()" << std::endl;

        delete[] data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const String& str) {
        return stream << '"' << str.data_ << '"';
    }

//private:
    char* data_ = nullptr;
    size_t size_ = 0;
};

String MakeString(String result) {
    return result;
}

void PrintString(String str) {
    std::cout << str << std::endl;
}

int main() {
    String hello("hello");
    String world("world");

    String hello_world;
    hello_world = std::move(hello);

    std::cout << hello_world << std::endl;

    return 0;
}
