#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>

struct Student
{
    std::string m_name;
    std::string m_surname;
    std::string m_email;
    int m_birth_year;
    int m_birth_month;
    int m_birth_day;
    std::string m_group;
    float m_rating;
    std::string m_phone_number;
};

struct GroupStats {
    double total_rating = 0.0;
    size_t count = 0;

    double average() const {
        return count > 0 ? (total_rating / count) : 0.0;
    }
};

std::vector<Student> loadStudentsFromCSV(const std::string& filename, size_t max_records = 0);

#endif // STUDENT_H
