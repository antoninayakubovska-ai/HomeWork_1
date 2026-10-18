#include "student.h"
#include <iostream>

#include "task1_1.cpp"
#include "task1_2.cpp"
#include "task1_3.cpp"

int main()
{
    std::string filepath = "data/students.csv";
    std::vector<Student> all_students = loadStudentsFromCSV(filepath);
    std::cout << "Успішно зчитано всіх студентів: " << all_students.size() << std::endl;

    if (all_students.empty())
    {
        std::cerr << "Помилка: База даних порожня або файл не знайдено" << std::endl;
        return 1;
    }

    const auto& s = all_students[0];
    std::cout << "\nПерший студент у базі:\n";
    std::cout << "Ім'я: " << s.m_name << " " << s.m_surname << "\n";
    std::cout << "Email: " << s.m_email << "\n";
    std::cout << "Дата народження: " << s.m_birth_day << "." << s.m_birth_month << "." << s.m_birth_year << "\n";
    std::cout << "Група: " << s.m_group << "\n";
    std::cout << "Рейтинг: " << s.m_rating << "\n";
    std::cout << "Телефон: " << s.m_phone_number << "\n";

    DatabaseVariantA db_a;
    DatabaseVariantB db_b;
    DatabaseVariantC db_c;

    db_a.init(all_students);
    db_b.init(all_students);
    db_c.init(all_students);

    std::string test_email = all_students[0].m_email;

    std::cout << "\nВаріант A Vector + Unordered Map" << std::endl;
    auto top100_a = db_a.get_top_100();
    std::cout << "Top 100 " << top100_a.size() << " студентів.\n";
    std::cout << "Update Rating " << (db_a.update_rating(test_email, 99.9f) ? "Успішно" : "Помилка") << "\n";
    auto best_a = db_a.get_best_groups();
    std::cout << "Best Group " << (best_a.empty() ? "-" : best_a[0]) << std::endl;

    std::cout << "\nВаріант B Map + Multiset" << std::endl;
    auto top100_b = db_b.get_top_100();
    std::cout << "Top 100 " << top100_b.size() << " студентів.\n";
    std::cout << "Update Rating " << (db_b.update_rating(test_email, 99.9f) ? "Успішно" : "Помилка") << "\n";
    auto best_b = db_b.get_best_groups();
    std::cout << "Best Group " << (best_b.empty() ? "-" : best_b[0]) << std::endl;

    std::cout << "\nВаріант C Unordered Map + Multiset" << std::endl;
    auto top100_c = db_c.get_top_100();
    std::cout << "Top 100 " << top100_c.size() << " студентів.\n";
    std::cout << "Update Rating " << (db_c.update_rating(test_email, 99.9f) ? "Успішно" : "Помилка") << "\n";
    auto best_c = db_c.get_best_groups();
    std::cout << "Best Group " << (best_c.empty() ? "-" : best_c[0]) << std::endl;

    return 0;
}
