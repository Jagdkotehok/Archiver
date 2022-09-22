#include "scorer.h"

#include <unordered_map>

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::vector<const Event*> events_pointers;
    for (const Event& event : events) {
        events_pointers.emplace_back(&event);
    }
    std::sort(events_pointers.begin(), events_pointers.end(),
              [&](const Event* l, const Event* r) { return l->time < r->time; });
    std::unordered_map<std::string, std::unordered_map<std::string, bool>>
        student_task_state;  /// 0 -- failed   1 -- success
    std::unordered_map<std::string, std::unordered_map<std::string, bool>>
        student_merge_request_state;  /// 1 -- open   0 -- closed
    for (const Event* event_pointer : events_pointers) {
        if (event_pointer->time > score_time) {
            break;
        }
        EventType event_type = event_pointer->event_type;
        if (event_type == EventType::MergeRequestOpen) {
            student_merge_request_state[event_pointer->student_name][event_pointer->task_name] = true;
        } else if (event_type == EventType::MergeRequestClosed) {
            student_merge_request_state[event_pointer->student_name][event_pointer->task_name] = false;
        } else if (event_type == EventType::CheckFailed) {
            student_task_state[event_pointer->student_name][event_pointer->task_name] = false;
        } else {
            student_task_state[event_pointer->student_name][event_pointer->task_name] = true;
        }
    }
    ScoreTable score_table = {};
    for (const auto& [student, tasks] : student_task_state) {
        for (const auto& [task, task_state] : tasks) {
            if (task_state && !student_merge_request_state[student][task]) { /// task accepted and merge_request closed
                score_table[student].emplace(task);
            }
        }
    }
    return score_table;  // test
}
