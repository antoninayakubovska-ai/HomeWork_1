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

void radixSortGroups(vector<Student>& arr)
{
    int max_len = 6;
    vector<Student> output(arr.size());

    for (int pos = max_len - 1; pos >= 0; --pos)
    {
        int count[256] = {0};

        for (const auto& s : arr) {
            char c = (pos < s.m_group.size()) ? s.m_group[pos] : 0;
            count[static_cast<unsigned char>(c)]++;
        }

        for (int i = 1; i < 256; ++i)
        {
            count[i] += count[i - 1];
        }

        for (int i = arr.size() - 1; i >= 0; --i)
        {
            char c = (pos < arr.size() && pos < arr[i].m_group.size()) ? arr[i].m_group[pos] : 0;
            output[count[static_cast<unsigned char>(c)] - 1] = arr[i];
            count[static_cast<unsigned char>(c)]--;
        }

        arr = output;
    }
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
    radixSortGroups(students_radix);
    auto end_radix = high_resolution_clock::now();
    auto time_radix = duration_cast<milliseconds>(end_radix - start_radix).count();

    cout << "2. Radix Sort виконано за " << time_radix << " мс\n\n";

    //результат найкращого
    string output_file = "sorted_students_s5.csv";
    cout << "Збереження результатів у файл " << output_file << "\n";
    saveStudentsToCSV(output_file, students_radix);

    return 0;
}
