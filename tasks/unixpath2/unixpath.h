#pragma once

#include <string>
#include <string_view>
#include <vector>

class UnixPath {
private:
    size_t FindNextSlash(std::string_view s, size_t pos) const {
        while (pos < s.size() && s[pos] != '/') {
            ++pos;
        }
        return pos;
    }
    size_t FindNextNotSlash(std::string_view s, size_t pos) const {
        while (pos < s.size() && s[pos] == '/') {
            ++pos;
        }
        return pos;
    }
    void ProcessPath(std::string_view path, std::vector<std::string_view> &resulting_path) {
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

    std::vector<std::string_view> working_dir_;
    std::vector<std::string_view> starting_dir_;

public:
    explicit UnixPath(std::string_view initial_dir) {
        working_dir_ = starting_dir_ = {};
        ProcessPath(initial_dir, working_dir_);
        ProcessPath(initial_dir, starting_dir_);
    }

    void ChangeDirectory(std::string_view path) {
        if (!path.empty() && path[0] == '/') {  /// path is absolute, so no need to consider previous path
            working_dir_.clear();
        }
        ProcessPath(path, working_dir_);
    }

    std::string GetAbsolutePath() const {
        std::string normalized_path = "/";
        for (size_t i = 0; i < working_dir_.size(); ++i) {
            if (i != 0) {  /// not first word
                normalized_path += "/";
            }
            normalized_path += working_dir_[i];
        }
        return normalized_path;
    }
    std::string GetRelativePath() const {
        std::string relative_path;
        size_t equal_prefix_length = 0;
        while (equal_prefix_length < std::min(working_dir_.size(), starting_dir_.size())) {
            if (working_dir_[equal_prefix_length] == starting_dir_[equal_prefix_length]) {
                ++equal_prefix_length;
            } else {
                break;
            }
        }
        for (size_t i = equal_prefix_length; i < starting_dir_.size(); ++i) {
            if (i != equal_prefix_length) {
                relative_path += "/";
            }
            relative_path += "..";
        }
        if (relative_path.empty()) {
            relative_path += ".";
        }
        for (size_t i = equal_prefix_length; i < working_dir_.size(); ++i) {
            relative_path += "/";
            relative_path += working_dir_[i];
        }
        return relative_path;
    }
};
