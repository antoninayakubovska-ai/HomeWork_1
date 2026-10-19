#include "student.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

#include "task1_1.cpp"
#include "task1_2.cpp"
#include "task1_3.cpp"

using namespace std;
using namespace std::chrono;


struct Operation
{
    int type;
    std::string email;
    float new_rating;
};

template <typename DB>
size_t run_benchmark(DB& db, const vector<Student>& students, const vector<Operation>& ops)
{
    db.init(students);
    size_t ops_executed = 0;
    size_t ops_count = ops.size();

    auto start_time = high_resolution_clock::now();
    auto end_time = start_time + seconds(10);

    while (true)
    {
        if (ops_executed % 100 == 0)
        {
            if (high_resolution_clock::now() >= end_time)
            {
                break;
            }
        }

        const Operation& op = ops[ops_executed % ops_count];

        if (op.type == 1)
        {
            db.get_top_100();
        } else if (op.type == 2)
        {
            db.update_rating(op.email, op.new_rating);
        } else if (op.type == 3)
        {
            db.get_best_groups();
        }

        ops_executed++;
    }
    return ops_executed;
}

int main()
{
    string filepath = "data/students.csv";

    vector<Student> students = loadStudentsFromCSV(filepath);
    if (students.empty())
    {
        cerr << "Помилка: база порожня" << endl;
        return 1;
    }

    cout << "Завантажено " << students.size() << " студентів.\n";
    cout << "Генерація пулу операцій у співвідношенні A:B:C = 2:100:30\n";

    mt19937 rng(42);
    discrete_distribution<int> dist({2, 100, 30});
    uniform_int_distribution<size_t> student_dist(0, students.size() - 1);
    uniform_real_distribution<float> rating_dist(0.0f, 100.0f);

    size_t PREGEN_SIZE = 1000000;
    vector<Operation> ops(PREGEN_SIZE);

    for (size_t i = 0; i < PREGEN_SIZE; ++i)
    {
        ops[i].type = dist(rng) + 1;
        if (ops[i].type == 2)
        {
            ops[i].email = students[student_dist(rng)].m_email;
            ops[i].new_rating = rating_dist(rng);
        }
    }

    cout << "\nЗапуск бенчмарку, очікування 30 секунд...\n";

    DatabaseVariantA db_a;
    DatabaseVariantB db_b;
    DatabaseVariantC db_c;

    cout << "Варіант A...\n";
    size_t count_a = run_benchmark(db_a, students, ops);

    cout << "Варіант B...\n";
    size_t count_b = run_benchmark(db_b, students, ops);

    cout << "Варіант C...\n";
    size_t count_c = run_benchmark(db_c, students, ops);

    cout << "РЕЗУЛЬТАТИ\n";
    cout << "Варіант A: " << count_a << "\n";
    cout << "Варіант B: " << count_b << "\n";
    cout << "Варіант C: " << count_c << "\n";

    return 0;
}
