#pragma once

#include <forward_list>
#include <string>
#include <tuple>

using Value = std::string;
using Iterator = std::forward_list<Value>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class Zipped {
public:
    class ZipIterator {
    public:
        Iterator iter1_;
        Iterator iter2_;

        ZipIterator(Iterator iter1, Iterator iter2);

        ZippedPair operator*() const;

        ZipIterator& operator++();

        bool operator==(const ZipIterator& other) const;
        bool operator!=(const ZipIterator& other) const;
    };

    Zipped(ZipIterator list_begin, ZipIterator list_end);

    ZipIterator begin_;
    ZipIterator end_;

    ZipIterator begin() const;  // NOLINT
    ZipIterator end() const;    // NOLINT
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
