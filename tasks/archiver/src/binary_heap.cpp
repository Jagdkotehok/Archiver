//
// Created by Kotehok3 on 23.10.2022.
//

#include <functional>

template <typename T>
class BinaryHeap {
private:
    std::function<bool(const T&, const T&)> f_;

    std::vector<T> data_;

    void HeapUp() {
        size_t v = data_.size() - 1;
        while (v) {

            size_t parent = (v - 1) / 2;
            if (f_(data_[v], data_[parent])) {
                std::swap(data_[v], data_[parent]);
            }

            v = parent;
        }
    }

    void HeapDown() {
        size_t v = 0;
        while (v < data_.size()) {
            size_t left = 2 * v + 1;
            size_t right = 2 * v + 2;
            if (right < data_.size()) {  /// v have 2 child
                size_t id_best = 0;

                if (f_(data_[left], data_[right])) {
                    id_best = left;
                } else {
                    id_best = right;
                }

                if (f_(data_[id_best], data_[v])) {
                    std::swap(data_[id_best], data_[v]);
                    std::swap(id_best, v);
                } else {
                    return;
                }
            } else if (left < data_.size()) {  /// v have 1 child
                if (f_(data_[left], data_[v])) {
                    std::swap(data_[left], data_[v]);
                    std::swap(left, v);
                } else {
                    return;
                }
            }
        }
    }

public:
    explicit BinaryHeap(std::function<bool(const T&, const T&)> f) : f_(f) {
        data_ = {};
    }

    T GetBest() {
        return data_[0];
    }

    void RemoveBest() {
        std::swap(data_[0], data_[data_.size() - 1]);
        data_.pop_back();
        HeapDown();
    }

    void Add(const T& elem) {
        data_.emplace_back(elem);
        HeapUp();
    }

    size_t Size() {
        return data_.size();
    }
};