#include "password.h"
#include <cctype>

const size_t MIN_SIZE = 8;
const size_t MAX_SIZE = 14;

const char MIN_SYMBOL = static_cast<char>(33);
const char MAX_SYMBOL = static_cast<char>(126);

bool ValidatePassword(const std::string& password) {
    size_t length = password.size();
    if (length < MIN_SIZE || length > MAX_SIZE) {
        return false;
    }
    int cnt_lower = 0;
    int cnt_upper = 0;
    int cnt_digit = 0;
    int cnt_other = 0;
    for (const char c : password) {
        if (c < MIN_SYMBOL || c > MAX_SYMBOL) {
            return false;
        }
        if (std::islower(c)) {
            cnt_lower = 1;
        } else if (std::isupper(c)) {
            cnt_upper = 1;
        } else if (std::isdigit(c)) {
            cnt_digit = 1;
        } else {
            cnt_other = 1;
        }
    }
    if (cnt_lower + cnt_upper + cnt_digit + cnt_other >= 3) {
        return true;
    }
    return false;
}
