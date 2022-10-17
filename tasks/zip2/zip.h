#pragma once

#include <iterator>

template <typename Iterator1, typename Iterator2>
class Zipped {
public:
    class ZipIterator {
    public:
        Iterator1 iter1_;
        Iterator2 iter2_;

        ZipIterator(Iterator1 iter1, Iterator2 iter2) : iter1_(iter1), iter2_(iter2) {
        }

        auto operator*() {
            auto& first = *iter1_;
            auto& second = *iter2_;
            return std::make_pair(first, second);
        }

        ZipIterator& operator++() {
            ++iter1_;
            ++iter2_;
            return *this;
        }

        bool operator==(const ZipIterator& other) const {
            return (iter1_ == other.iter1_) || (iter2_ == other.iter2_);
        }

        bool operator!=(const ZipIterator& other) const {
            return !(*this == other);
        }
    };

    ZipIterator begin_;
    ZipIterator end_;

    const ZipIterator& begin() const {  // NOLINT
        return begin_;
    }

    const ZipIterator& end() const {  // NOLINT
        return end_;
    }

    Zipped(Iterator1 begin1, Iterator1 end1, Iterator2 begin2, Iterator2 end2)
        : begin_(begin1, begin2), end_(end1, end2) {
    }
};

template <typename Sequence1, typename Sequence2>
auto Zip(const Sequence1& sequence1, const Sequence2& sequence2) {
    auto begin1 = std::begin(sequence1);
    auto end1 = std::end(sequence1);
    auto begin2 = std::begin(sequence2);
    auto end2 = std::end(sequence2);
    auto result = Zipped<decltype(begin1), decltype(begin2)>(begin1, end1, begin2, end2);
    return result;
}
