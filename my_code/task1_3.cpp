//Варіант C: std::unordered_map + std::multiset
#include "student.h"
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

class DatabaseVariantC {
private:
    std::unordered_map<std::string, Student> m_email_map;
    std::multiset<std::pair<float, std::string>, std::greater<std::pair<float, std::string>>> m_rating_index;
    std::unordered_map<std::string, GroupStats> m_group_stats;

public:
    void init(const std::vector<Student>& data)
    {
        m_email_map.clear();
        m_rating_index.clear();
        m_group_stats.clear();

        for (const auto& s : data)
        {
            m_email_map[s.m_email] = s;
            m_rating_index.insert({s.m_rating, s.m_email});
            m_group_stats[s.m_group].total_rating += s.m_rating;
            m_group_stats[s.m_group].count += 1;
        }
    }

    //100 студентів з найвищим рейтингом
    std::vector<Student> get_top_100() const
    {
        std::vector<Student> result;
        result.reserve(100);
        size_t count = 0;

        for (auto it = m_rating_index.begin(); it != m_rating_index.end() && count < 100; ++it, ++count)
        {
            auto student_it = m_email_map.find(it->second);
            if (student_it != m_email_map.end())
            {
                result.push_back(student_it->second);
            }
        }
        return result;
    }

    //рейтинг за email
    bool update_rating(const std::string& email, float new_rating) {
        auto it = m_email_map.find(email);
        if (it == m_email_map.end()) return false;

        float old_rating = it->second.m_rating;
        const std::string& group = it->second.m_group;

        //O(log N)
        auto r_it = m_rating_index.find({old_rating, email});
        if (r_it != m_rating_index.end()) {
            m_rating_index.erase(r_it);
        }
        m_rating_index.insert({new_rating, email});

        //O(1)
        m_group_stats[group].total_rating += (new_rating - old_rating);
        it->second.m_rating = new_rating;
        return true;
    }

    //найбільший середній рейтинг
    std::vector<std::string> get_best_groups() const
    {
        double max_avg = -1.0;
        std::vector<std::string> best_groups;

        for (const auto& [group, stat] : m_group_stats)
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
