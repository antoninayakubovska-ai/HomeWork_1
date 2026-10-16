#include "student.h"
#include <iostream>

int main() {
    std::string filepath = "data/students.csv";
    std::vector<Student> all_students = loadStudentsFromCSV(filepath);
    std::cout << "Успішно зчитано всіх студентів: " << all_students.size() << std::endl;

    std::vector<Student> students_100 = loadStudentsFromCSV(filepath, 100);
    std::cout << "Зчитано для експерименту 100: " << students_100.size() << std::endl;

    if (!all_students.empty())
    {
        const auto& s = all_students[0];
        std::cout << "\nПерший студент у базі:\n";
        std::cout << "Ім'я: " << s.m_name << " " << s.m_surname << "\n";
        std::cout << "Email: " << s.m_email << "\n";
        std::cout << "Дата народження: " << s.m_birth_day << "." << s.m_birth_month << "." << s.m_birth_year << "\n";
        std::cout << "Група: " << s.m_group << "\n";
        std::cout << "Рейтинг: " << s.m_rating << "\n";
        std::cout << "Телефон: " << s.m_phone_number << std::endl;
    }

    return 0;
}
