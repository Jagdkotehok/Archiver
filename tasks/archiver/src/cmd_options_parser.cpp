//
// Created by Kotehok3 on 22.10.2022.
//

#include <string>
#include <vector>

class OptionsParser {
public:
    class Option {
    public:
        std::string name;
        std::vector<std::string> arguments;
    };

    std::vector<Option> options;

private:
    void AddOption_(Option& opt){};  /// NOLINT

    template <typename... Args>
    void AddOption_(Option& opt, const std::string& arg, Args... args) {  /// NOLINT
        opt.arguments.emplace_back(arg);
        AddOption_(opt, args...);
    }

public:
    template <typename... Args>
    void AddOption(const std::string& name, Args... args) {
        Option cur_option;
        cur_option.name = name;
        AddOption_(cur_option, args...);
        options.emplace_back(cur_option);
    }

    std::string FindOption(int argc, char** argv) {
        size_t args_cnt = static_cast<size_t>(argc);

        for (const Option& cur_option : options) {

            size_t argv_pos = 1;
            int state = 0;  /// -1 -- not this option   0 -- might be this option   1 -- definitely this option

            for (const std::string& arg : cur_option.arguments) {
                std::string argv_cur_arg = static_cast<std::string>(argv[argv_pos]);

                if (argv_pos == args_cnt) {  /// already exceeded given args count
                    state = -1;
                    break;
                }

                if (arg == "any") {  /// any arbitrary word
                    ++argv_pos;
                    continue;
                } else if (arg == "any...") {  /// arbitrary count of arbitrary words, but at least one
                    state = 1;
                    break;
                } else {  /// given parameter
                    if (argv_cur_arg != arg) {
                        state = -1;
                        break;
                    }
                    ++argv_pos;
                    continue;
                }
            }

            if (state == 0) {
                if (argv_pos != args_cnt) {
                    state = -1;
                }
            }

            if (state == -1) {
                continue;
            } else {
                return cur_option.name;
            }
        }
        return "-1";
    }
};