#include "sort_students.h"
#include <algorithm>

bool operator<(const Date& left, const Date& right) {
    return std::tie(left.year, left.month, left.day) < std::tie(right.year, right.month, right.day);
}

void SortStudents(std::vector<Student>& students, SortKind sortKind) {
    std::sort(students.begin(), students.end(), [&](const Student& left, const Student& right) {
        if (sortKind == SortKind::Date) {
            return std::tie(left.birth_date, left.last_name, left.name) < std::tie(right.birth_date, right.last_name, right.name);
        } else {
            return std::tie(left.last_name, left.name, left.birth_date) < std::tie(right.last_name, right.name, right.birth_date);
        }
    });
}
