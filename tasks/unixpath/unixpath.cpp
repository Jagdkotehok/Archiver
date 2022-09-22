#include "unixpath.h"

#include <vector>

size_t FindNextSlash(std::string_view s, size_t start_pos) {
    for (size_t pos = start_pos + 1; pos < s.size(); ++pos) {
        if (s[pos] == '/') {
            return pos;
        }
    }
    return s.size();
}

size_t FindNextNotSlash(std::string_view s, size_t start_pos) {
    for (size_t pos = start_pos; pos < s.size(); ++pos) {
        if (s[pos] != '/') {
            return pos;
        }
    }
    return s.size();
}

void ProcessPath(std::string_view path, std::vector<std::string_view>& resulting_path) {
    size_t current_pos = 0;
    while (current_pos < path.size()) {
        size_t not_slash_pos = FindNextNotSlash(path, current_pos);
        if (not_slash_pos == path.size()) {
            break;
        }
        size_t slash_pos = FindNextSlash(path, not_slash_pos);
        std::string_view current_word = path.substr(not_slash_pos, slash_pos - not_slash_pos);
        if (current_word == std::string_view("..")) {
            if (!resulting_path.empty()) {
                resulting_path.pop_back();
            }
        } else if (current_word == std::string_view(".")) {  /// doing nothing, staying in same directory

        } else {
            resulting_path.emplace_back(current_word);
        }
        current_pos = slash_pos;
    }
}

std::string NormalizePath(std::string_view current_working_dir, std::string_view path) {
    std::vector<std::string_view> resulting_path;
    ProcessPath(current_working_dir, resulting_path);
    if (!path.empty() &&
        path[0] == '/') {  /// path is absolute, not relative, so no need to consider current_working_dir
        resulting_path.clear();
    }
    ProcessPath(path, resulting_path);
    std::string normalized_path = "/";
    for (size_t i = 0; i < resulting_path.size(); ++i) {
        if (i != 0) {  /// not first word
            normalized_path += "/";
        }
        std::string_view word = resulting_path[i];
        for (size_t j = 0; j < word.size(); ++j) {
            normalized_path += word[j];
        }
    }
    return normalized_path;
}
