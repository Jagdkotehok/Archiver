#pragma once

#include <string_view>
#include <vector>
#include <map>
#include <set>
#include <cctype>
#include <numeric>
#include <cmath>

class SearchEngine {
private:
    struct StringViewLowerComparator {
        bool operator()(const std::string_view& l, const std::string_view& r) const {
            size_t size = std::min(l.size(), r.size());
            for (size_t i = 0; i < size; ++i) {
                if (std::tolower(l[i]) != std::tolower(r[i])) {
                    return std::tolower(l[i]) < std::tolower(r[i]);
                }
            }
            /// prefixes are equal, so left is lower only if it is shorter
            return l.size() < r.size();
        }
    };

    size_t FindNextEOL(size_t pos, std::string_view& text) {
        while (pos < text.size()) {
            if (text[pos] == '\n') {
                break;
            }
            ++pos;
        }
        return pos;
    }

    std::vector<std::string_view> documents_;  /// list of all documents
    std::vector<size_t> total_words_;          /// cnt of words in i-th document
    std::vector<std::map<std::string_view, size_t, StringViewLowerComparator>>
        word_counter_;  /// cnt of given word in i-th document
    std::map<std::string_view, size_t, StringViewLowerComparator>
        documents_with_word_;  /// cnt of documents with given word

    long double CalcTF(std::string_view word, size_t total_words,
                       const std::map<std::string_view, size_t, StringViewLowerComparator>& doc_word_counter) const {
        if (doc_word_counter.find(word) == doc_word_counter.end()) {
            return static_cast<long double>(0);
        }
        if (total_words == 0) {
            return static_cast<long double>(0);
        }
        return static_cast<long double>(doc_word_counter.at(word)) / static_cast<long double>(total_words);
    }

    long double CalcIDF(
        std::string_view word, size_t total_documents,
        const std::map<std::string_view, size_t, StringViewLowerComparator>& documents_with_word) const {
        if (total_documents == 0) {
            return static_cast<long double>(0);
        }
        if (documents_with_word.find(word) == documents_with_word.end()) {
            return static_cast<long double>(0);
        }
        return std::log(static_cast<long double>(total_documents) /
                        static_cast<long double>(documents_with_word.at(word)));
    }

public:
    void BuildIndex(std::string_view text) {
        documents_.clear();
        total_words_.clear();
        word_counter_.clear();
        documents_with_word_.clear();

        size_t text_pos = 0;
        while (text_pos < text.size()) {
            size_t eol_pos = FindNextEOL(text_pos, text);
            documents_.emplace_back(text.substr(text_pos, eol_pos - text_pos));
            text_pos = eol_pos + 1;
        }
        for (const std::string_view document : documents_) {

            size_t total_words_in_doc = 0;  /// cnt of words in current document
            std::map<std::string_view, size_t, StringViewLowerComparator>
                word_counter_in_doc;  /// cnt of give word in current document
            std::set<std::string_view, StringViewLowerComparator>
                unique_words_in_doc;  /// only 1 copy of each word in document

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
                ++documents_with_word_[word];
            }
            total_words_.emplace_back(total_words_in_doc);
            word_counter_.emplace_back(word_counter_in_doc);
        }
    }
    std::vector<std::string_view> Search(std::string_view query, size_t results_count) const {
        std::vector<long double> tfidf_value(documents_.size(), 0);  /// TF-IDF value for i-th document
        std::set<std::string_view> unique_query_words;               /// only 1 copy of each word in query

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
            for (size_t i = 0; i < documents_.size(); ++i) {
                long double tf_value = CalcTF(query_word, total_words_[i], word_counter_[i]);
                long double idf_value = CalcIDF(query_word, documents_.size(), documents_with_word_);
                tfidf_value[i] += tf_value * idf_value;
            }
        }

        std::vector<size_t> order_of_documents(documents_.size());  /// indices of documents
        std::iota(order_of_documents.begin(), order_of_documents.end(), 0);
        std::sort(order_of_documents.begin(), order_of_documents.end(), [&](size_t l, size_t r) {
            if (tfidf_value[l] != tfidf_value[r]) {
                return tfidf_value[l] > tfidf_value[r];
            }
            return l < r;
        });  /// now those indices are sorted by increasing of TF-IDF value

        std::vector<std::string_view> sorted_documents;
        for (size_t i = 0; i < order_of_documents.size(); ++i) {
            if (sorted_documents.size() == results_count) {
                break;
            }
            size_t document_id = order_of_documents[i];
            if (tfidf_value[document_id] > static_cast<long double>(0)) {
                sorted_documents.emplace_back(documents_[document_id]);
            }
        }

        return sorted_documents;
    }
};
