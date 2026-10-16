#include "student.h"
#include <fstream>
#include <sstream>
#include <algorithm>

static std::string trim(const std::string& str)
{

    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.subst(start, end-start+1);

}

std::vector<Student> loadStudentsFromCSV(const std::string& filename, size_t max_records)
{
    std::vector<Student> students;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cerr << "Помилка відкриття файлу: " << filename << std::endl;
        return students;
    }

    std::string line;
    bool is_first_line = true;

    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> row;

        while (std::getline(ss, token, ','))
        {
            row.push_back(trim(token));
        }

        if (is_first_line)
        {
            is_first_line = false;
            if (!row.empty() && (row[0] == "m_name" || row[0] == "Name" || row[0] == "name"))
            {
                continue;
            }
        }

        if (row.size() < 9)
        {
            continue;
        }

        try
        {
            Student student;
            student.m_name = row[0];
            student.m_surname = row[1];
            student.m_email = row[2];
            student.m_birth_year = std::stoi(row[3]);
            student.m_birth_month = std::stoi(row[4]);
            student.m_birth_day = std::stoi(row[5]);
            student.m_group = row[6];
            student.m_rating = std::stof(row[7]);
            student.m_phone_number = row[8];

            students.push_back(student);

            if (max_records > 0 && students.size() >= max_records)
            {
                break;
            }
        } catch (const std::exception& e)
        {
            std::cerr << "Помилка конвертації рядка: " << line << " | " << e.what() << std::endl;
        }
    }

    file.close();
    return students;
}
