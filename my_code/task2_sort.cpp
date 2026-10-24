#include "student.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void saveStudentsToCSV(const string& filename, const vector<Student>& students)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cerr << "Не вдалося створити файл " << filename << endl;
        return;
    }

    file << "m_name,m_surname,m_email,m_birth_year,m_birth_month,m_birth_day,m_group,m_rating,m_phone_number\n";
    for (const auto& s : students)
    {
        file << s.m_name << "," << s.m_surname << "," << s.m_email << ","
             << s.m_birth_year << "," << s.m_birth_month << "," << s.m_birth_day << ","
             << s.m_group << "," << s.m_rating << "," << s.m_phone_number << "\n";
    }
    file.close();
}

static inline uint32_t packKey(const std::string& g)
{
    return (((uint32_t)(g[0]-'A')*26 + (g[1]-'A'))*26 + (g[2]-'A'))*100
           + (g[4]-'0')*10 + (g[5]-'0');
}

void radixPacked(std::vector<Student>& arr)
{
    size_t n = arr.size();
    std::vector<std::pair<uint32_t,uint32_t>> a(n), b(n);

    for (size_t i = 0; i < n; i++)
        a[i] = { packKey(arr[i].m_group), (uint32_t)i };

    for (int shift = 0; shift < 24; shift += 8)
    {
        size_t cnt[257] = {0};
        for (auto& p : a) cnt[((p.first >> shift) & 255) + 1]++;
        for (int i = 0; i < 256; i++) cnt[i+1] += cnt[i];
        for (auto& p : a) b[cnt[(p.first >> shift) & 255]++] = p;
        a.swap(b);
    }

    std::vector<Student> out;
    out.reserve(n);
    for (auto& p : a) out.push_back(std::move(arr[p.second]));
    arr.swap(out);
}
int main() {
    string filepath = "data/students.csv";
    vector<Student> original_students = loadStudentsFromCSV(filepath);

    if (original_students.empty()) {
        cerr << "Помилка: база порожня" << endl;
        return 1;
    }

    cout << "Кількість студентів: " << original_students.size() << "\n\n";

    //стандартне сортування
    vector<Student> students_std = original_students;

    auto start_std = high_resolution_clock::now();
    sort(students_std.begin(), students_std.end(), [](const Student& a, const Student& b)
    {
        return a.m_group < b.m_group;
    });
    auto end_std = high_resolution_clock::now();
    auto time_std = duration_cast<milliseconds>(end_std - start_std).count();

    cout << "1. std::sort виконано за " << time_std << " мс\n";

    //Radix Sort
    vector<Student> students_radix = original_students;

    auto start_radix = high_resolution_clock::now();
    radixPacked(students_radix);
    auto end_radix = high_resolution_clock::now();
    auto time_radix = duration_cast<milliseconds>(end_radix - start_radix).count();

    cout << "2. Radix Sort виконано за " << time_radix << " мс\n\n";

    //результат найкращого
    string output_file = "sorted_students_s5.csv";
    cout << "Збереження результатів у файл " << output_file << "\n";
    saveStudentsToCSV(output_file, students_radix);

    return 0;
}
