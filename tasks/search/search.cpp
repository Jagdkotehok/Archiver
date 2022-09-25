#include "search.h"

#include <cctype>
#include <map>
#include <set>
#include <cmath>
#include <numeric>

struct StringViewLowerComparator {
    bool operator()(const std::string_view left, const std::string_view right) const {
        size_t size = std::min(left.size(), right.size());
        for (size_t i = 0; i < size; ++i) {
            if (std::tolower(left[i]) != std::tolower(right[i])) {
                return std::tolower(left[i]) < std::tolower(right[i]);
            }
        }
        /// prefixes are equal, so left is lower only if it`s shorter
        return left.size() < right.size();
    }
};

long double CalcTF(std::string_view word, size_t total_words,
                   const std::map<std::string_view, size_t, StringViewLowerComparator>& doc_word_counter) {
    size_t equal_words = 0;
    if (doc_word_counter.find(word) != doc_word_counter.end()) {
        equal_words = doc_word_counter.at(word);
    }
    if (total_words == 0) {
        return static_cast<long double>(0);
    } else {
        return static_cast<long double>(equal_words) / static_cast<long double>(total_words);
    }
}

long double CalcIDF(std::string_view word, size_t total_documents,
                    const std::map<std::string_view, size_t, StringViewLowerComparator>& documents_with_word) {
    if (total_documents == 0) {
        return static_cast<long double>(0);
    }
    if (documents_with_word.find(word) == documents_with_word.end()) {
        return static_cast<long double>(0);
    }
    return std::log(static_cast<long double>(total_documents) / static_cast<long double>(documents_with_word.at(word)));
}

size_t FindNextEOL(size_t pos, std::string_view text) {
    while (pos < text.size()) {
        if (text[pos] == '\n') {
            break;
        }
        ++pos;
    }
    return pos;
}

std::vector<std::string_view> Search(std::string_view text, std::string_view query, size_t results_count) {
    std::vector<std::string_view> documents;
    size_t text_pos = 0;
    while (text_pos < text.size()) {
        size_t eol_pos = FindNextEOL(text_pos, text);
        documents.emplace_back(text.substr(text_pos, eol_pos - text_pos));
        text_pos = eol_pos + 1;
    }

    /// for TF
    std::vector<size_t> total_words;
    std::vector<std::map<std::string_view, size_t, StringViewLowerComparator>> word_counter;
    /// for IDF
    std::map<std::string_view, size_t, StringViewLowerComparator> documents_with_word;

    for (const std::string_view document : documents) {
        /// for TF
        size_t total_words_in_doc = 0;
        std::map<std::string_view, size_t, StringViewLowerComparator> word_counter_in_doc;
        /// for IDF
        std::set<std::string_view, StringViewLowerComparator> unique_words_in_doc;

        size_t doc_pos = 0;
        while (doc_pos < document.size()) {
            if (!std::isalpha(document[doc_pos])) {
                ++doc_pos;
                continue;
            }
            size_t pos_left = doc_pos;
            size_t pos_right = doc_pos + 1;
            while (pos_right < document.size() && std::isalpha(document[pos_right])) {
                ++pos_right;
            }
            std::string_view word = document.substr(pos_left, pos_right - pos_left);

            unique_words_in_doc.emplace(word);
            ++word_counter_in_doc[word];
            ++total_words_in_doc;

            doc_pos = pos_right;
        }
        for (std::string_view word : unique_words_in_doc) {
            ++documents_with_word[word];
        }
        total_words.emplace_back(total_words_in_doc);
        word_counter.emplace_back(word_counter_in_doc);
    }

    std::vector<long double> tfidf_value(documents.size(), 0);
    std::set<std::string_view> unique_query_words;
    
    size_t query_pos = 0;
    while (query_pos < query.size()) {
        if (!std::isalpha(query[query_pos])) {
            ++query_pos;
            continue;
        }
        size_t pos_left = query_pos;
        size_t pos_right = pos_left + 1;
        while (pos_right < query.size() && std::isalpha(query[pos_right])) {
            ++pos_right;
        }
        std::string_view query_word = query.substr(pos_left, pos_right - pos_left);
        unique_query_words.emplace(query_word);
        query_pos = pos_right;
    }
    for (std::string_view query_word : unique_query_words) {
        for (size_t i = 0; i < documents.size(); ++i) {
            long double tf_value = CalcTF(query_word, total_words[i], word_counter[i]);
            long double idf_value = CalcIDF(query_word, documents.size(), documents_with_word);
            tfidf_value[i] += tf_value * idf_value;
        }
    }

    std::vector<size_t> order_of_documents(documents.size());
    std::iota(order_of_documents.begin(), order_of_documents.end(), 0);
    std::sort(order_of_documents.begin(), order_of_documents.end(), [&](size_t l, size_t r) {
        if (tfidf_value[l] != tfidf_value[r]) {
            return tfidf_value[l] > tfidf_value[r];
        }
        return l < r;
    });

    std::vector<std::string_view> sorted_documents;
    for (size_t i = 0; i < order_of_documents.size(); ++i) {
        if (sorted_documents.size() == results_count) {
            break;
        }
        size_t document_id = order_of_documents[i];
        if (tfidf_value[document_id] > static_cast<long double>(0)) {
            sorted_documents.emplace_back(documents[document_id]);
        }
    }

    return sorted_documents;
}
