#ifndef STUDENT_H
#define STUDENT_H


#include <string>
#include <vector>
#include <iostream>

struct Student
{
    std::string m_name;
    std::string m_surname;
    int m_birthday_year;
    int m_birthday_month;
    int m_birthday_day;
    std::string m_group;
    float m_rating;
    std::string m_phone_number;
};

///@param filename
///@param max_records

std::vector<Student> loadStudentFromCSV(const std::string& filename, size_t max_records = 0);


#endif
