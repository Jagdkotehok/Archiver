#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>

class Vector {
public:
    using ValueType = int;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;

    ValueType* data_;
    SizeType size_;
    SizeType capacity_;

    class Iterator {
    public:
        ValueType* elem_;

        explicit Iterator(ValueType* pointer) {
            elem_ = pointer;
        }
        Iterator() {
            elem_ = nullptr;
        }

        ValueType& operator*() const {
            return *elem_;
        }
        ValueType* operator->() const {
            return elem_;
        }

        Iterator& operator=(Iterator other) {
            elem_ = other.elem_;
            return *this;
        }

        Iterator& operator++() {
            ++elem_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator prev_version = *this;
            ++elem_;
            return prev_version;
        }
        Iterator& operator--() {
            --elem_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator prev_version = *this;
            --elem_;
            return prev_version;
        }

        Iterator operator+(DifferenceType shift) {
            Iterator result = *this;
            result.elem_ += shift;
            return result;
        }
        DifferenceType operator-(Iterator other) {
            return elem_ - other.elem_;
        }
        Iterator& operator+=(DifferenceType shift) {
            elem_ += shift;
            return *this;
        }
        Iterator& operator-=(DifferenceType shift) {
            elem_ -= shift;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return elem_ == other.elem_;
        }
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
        std::strong_ordering operator<=>(const Iterator& other) const {
            if (elem_ < other.elem_) {
                return std::strong_ordering::less;
            } else if (elem_ == other.elem_) {
                return std::strong_ordering::equal;
            } else {
                return std::strong_ordering::greater;
            }
        }
    };

    Vector() {
        size_ = 0;
        capacity_ = 0;
        data_ = nullptr;
    }
    explicit Vector(size_t size) {
        size_ = size;
        capacity_ = size;
        data_ = new ValueType[capacity_];
        for (SizeType i = 0; i < size; ++i) {
            data_[i] = 0;
        }
    }
    Vector(std::initializer_list<ValueType> list) {
        size_ = 0;
        capacity_ = list.size();
        data_ = new ValueType[capacity_];
        for (auto& x : list) {
            data_[size_++] = x;
        }
    }
    Vector(const Vector& other) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new ValueType[capacity_];
        for (SizeType i = 0; i < capacity_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    Vector& operator=(const Vector& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        delete[] data_;
        data_ = new ValueType[capacity_];
        for (SizeType i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        return *this;
    }

    ~Vector() {
        delete[] data_;
    }

    SizeType Size() const {
        return size_;
    }
    SizeType Capacity() const {
        return capacity_;
    }

    const ValueType* Data() const {
        return data_;
    }

    ValueType& operator[](size_t position) {
        return data_[position];
    }
    ValueType operator[](size_t position) const {
        return data_[position];
    }

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (SizeType i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
    std::strong_ordering operator<=>(const Vector& other) const {
        SizeType min_size = std::min(size_, other.size_);
        for (SizeType i = 0; i < min_size; ++i) {
            if (data_[i] < other.data_[i]) {
                return std::strong_ordering::less;
            }
        }
        /// prefixes are equal
        if (size_ < other.size_) {
            return std::strong_ordering::less;
        } else if (size_ == other.size_) {
            return std::strong_ordering::equal;
        } else {
            return std::strong_ordering::greater;
        }
    }

    void Reserve(SizeType new_capacity) {
        if (capacity_ < new_capacity) {
            capacity_ = new_capacity;
            ValueType* new_data = new ValueType[new_capacity];
            for (SizeType i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
        }
    }
    void Clear() {
        size_ = 0;
    }

    void PushBack(const ValueType& new_element) {
        if (size_ == capacity_) {
            if (capacity_ == 0) {
                Reserve(1);
            } else {
                Reserve(capacity_ * 2);
            }
        }
        data_[size_++] = new_element;
    }
    void PopBack() {
        --size_;
    }

    void Swap(Vector& other) {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    Iterator Begin() {
        return Iterator(data_);
    }
    Iterator End() {
        return Iterator(data_ + size_);
    }

    Iterator begin() {  // NOLINT
        return Begin();
    }
    Iterator end() {  // NOLINT
        return End();
    }
};
