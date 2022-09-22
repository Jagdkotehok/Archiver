#include "tests_checking.h"

#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<std::string> works_order;
    for (const StudentAction& student_action : student_actions) {
        if (student_action.side == Side::Top) {
            works_order.emplace_back(student_action.name);
        } else {
            works_order.emplace_front(student_action.name);
        }
    }
    std::vector<std::string> students_order;
    for (const size_t student_index : queries) {
        students_order.emplace_back(works_order[student_index - 1]);
    }
    return students_order;
}
