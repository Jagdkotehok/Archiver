#include "zip.h"

Zipped::ZipIterator::ZipIterator(Iterator iter1, Iterator iter2) {
    iter1_ = iter1;
    iter2_ = iter2;
}

ZippedPair Zipped::ZipIterator::operator*() const {
    return ZippedPair(*iter1_, *iter2_);
}

Zipped::ZipIterator& Zipped::ZipIterator::operator++() {
    ++iter1_;
    ++iter2_;
    return *this;
}

bool Zipped::ZipIterator::operator==(const Zipped::ZipIterator& other) const {
    return (iter1_ == other.iter1_) || (iter2_ == other.iter2_);
}

bool Zipped::ZipIterator::operator!=(const Zipped::ZipIterator& other) const {
    return !(*this == other);
}

Zipped::ZipIterator Zipped::begin() const {
    return begin_;
}

Zipped::ZipIterator Zipped::end() const {
    return end_;
}
Zipped::Zipped(Zipped::ZipIterator list_begin, Zipped::ZipIterator list_end) : begin_(list_begin), end_(list_end) {
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    Zipped::ZipIterator result_begin(a_begin, b_begin);
    Zipped::ZipIterator result_end(a_end, b_end);
    return Zipped(result_begin, result_end);
}
