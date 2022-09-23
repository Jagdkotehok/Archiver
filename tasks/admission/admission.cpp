#include "admission.h"

bool operator<(const Date& l, const Date& r) {
    return std::tie(l.year, l.month, l.day) < std::tie(r.year, r.month, r.day);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> applicants_pointers;
    for (const Applicant& applicant : applicants) {
        applicants_pointers.emplace_back(&applicant);
    }
    std::sort(applicants_pointers.begin(), applicants_pointers.end(), [&](const Applicant* l, const Applicant* r) {
        if (l->points != r->points) {
            return l->points > r->points;
        } else {
            return std::tie(l->student.birth_date, l->student.name) < std::tie(r->student.birth_date, r->student.name);
        }
    });
    std::unordered_map<std::string, size_t> university_students_left;
    for (const University& university : universities) {
        university_students_left[university.name] = university.max_students;
    }
    AdmissionTable admission_table = {};
    for (const Applicant* applicant : applicants_pointers) {
        for (const std::string& university : applicant->wish_list) {
            size_t& free_places = university_students_left[university];
            if (free_places > 0) {
                --free_places;
                admission_table[university].emplace_back(&applicant->student);
                break;
            }
        }
    }
    return admission_table;
}
