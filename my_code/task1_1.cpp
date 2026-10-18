//Варіант A: std::vector + std::unordered_map
#include "student.h"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

class DatabaseVariantA
{
private:
    std::vector<Student> m_students;
    std::unordered_map<std::string, size_t> m_email_to_index;

public:
    void init(const std::vector<Student>& data)
    {
        m_students = data;
        m_email_to_index.clear();
        for (size_t i = 0; i < m_students.size(); ++i)
        {
            m_email_to_index[m_students[i].m_email] = i;
        }
    }

    //100 студентів з найвищим рейтингом
    std::vector<Student> get_top_100() const
    {
        size_t count = std::min<size_t>(100, m_students.size());
        std::vector<const Student*> ptrs;
        ptrs.reserve(m_students.size());
        for (const auto& s : m_students)
        {
            ptrs.push_back(&s);
        }

        std::partial_sort(ptrs.begin(), ptrs.begin() + count, ptrs.end(),
            [](const Student* a, const Student* b)
            {
                return a->m_rating > b->m_rating;
            });

        std::vector<Student> result;
        result.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            result.push_back(*ptrs[i]);
        }
        return result;
    }

    //рейтинг за email
    bool update_rating(const std::string& email, float new_rating) {
        auto it = m_email_to_index.find(email);
        if (it == m_email_to_index.end()) return false;
        m_students[it->second].m_rating = new_rating;
        return true;
    }

    //найбільший середній рейтинг
    std::vector<std::string> get_best_groups() const
    {
        std::unordered_map<std::string, GroupStats> stats;
        for (const auto& s : m_students)
        {
            stats[s.m_group].total_rating += s.m_rating;
            stats[s.m_group].count += 1;
        }

        double max_avg = -1.0;
        std::vector<std::string> best_groups;
        for (const auto& [group, stat] : stats)
        {
            double avg = stat.average();
            if (avg > max_avg + 1e-6)
            {
                max_avg = avg;
                best_groups = {group};
            } else if (std::abs(avg - max_avg) <= 1e-6)
            {
                best_groups.push_back(group);
            }
        }
        return best_groups;
    }
};

