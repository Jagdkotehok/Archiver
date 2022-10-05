#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

using StudentName = std::string;
using TaskName = std::string;

using ScoreTable = std::map<StudentName, std::set<TaskName>>;

class Scorer {
private:
    std::map<StudentName, std::set<TaskName>> accepted_solves_;
    std::map<StudentName, std::set<TaskName>> opened_merge_requests_;

public:
    void OnCheckFailed(const StudentName& student_name, const TaskName& task_name) {
        accepted_solves_[student_name].erase(task_name);
        if (accepted_solves_[student_name].empty()) {
            accepted_solves_.erase(student_name);
        }
    }
    void OnCheckSuccess(const StudentName& student_name, const TaskName& task_name) {
        accepted_solves_[student_name].emplace(task_name);
    }
    void OnMergeRequestOpen(const StudentName& student_name, const TaskName& task_name) {
        opened_merge_requests_[student_name].emplace(task_name);
    }
    void OnMergeRequestClosed(const StudentName& student_name, const TaskName& task_name) {
        opened_merge_requests_[student_name].erase(task_name);
        if (opened_merge_requests_[student_name].empty()) {
            opened_merge_requests_.erase(student_name);
        }
    }
    void Reset() {
        accepted_solves_.clear();
        opened_merge_requests_.clear();
    }

    ScoreTable GetScoreTable() const {
        ScoreTable score_table;
        for (auto& [student, accepted_solves] : accepted_solves_) {
            std::set<TaskName> accepted_tasks;
            for (auto& task_name : accepted_solves) {
                bool is_task_accepted = false;
                if (opened_merge_requests_.find(student) ==
                    opened_merge_requests_.end()) {  /// no merge request for this student
                    is_task_accepted = true;
                } else {  /// there are some merge request for this student
                    if (opened_merge_requests_.at(student).find(task_name) ==
                        opened_merge_requests_.at(student).end()) {  /// there is no opened merge request for this task
                        is_task_accepted = true;
                    }
                }
                if (is_task_accepted) {
                    accepted_tasks.emplace(task_name);
                }
            }
            if (!accepted_tasks.empty()) {
                score_table[student] = accepted_tasks;
            }
        }
        return score_table;
    }
};
