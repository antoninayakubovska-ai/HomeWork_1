#include "student.h"
#include "task1_3.cpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <numeric>

using namespace std;
using namespace std::chrono;

vector<Student> generateData(const vector<Student>& base_data, size_t target_size){
    vector<Student> result;
    result.reserve(target_size);
    size_t base_size = base_data.size();

    for (size_t i = 0; i < target_size; ++i){
        Student s = base_data[i % base_size];
        if (i >= base_size) {
            s.m_email = to_string(i) + "_" + s.m_email;
        }
        result.push_back(s);
    }
    return result;
}

//Функція для обчислення середнього та стандартного відхилення
pair<double, double> calcStats(const vector<double>& times)
{
    double sum = accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    double sq_sum = inner_product(times.begin(), times.end(), times.begin(), 0.0);
    double stdev = sqrt(sq_sum / times.size() - mean * mean);
    return {mean, stdev};
}

int main()
{
    string filepath = "data/students.csv";
    vector<Student> base_data = loadStudentsFromCSV(filepath);
    if (base_data.empty()) {
        cerr << "Помилка: порожня база\n";
        return 1;
    }

    // 10 точок для графіка
    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 20000, 40000, 60000, 80000, 100000};
    int repetitions = 5;

    ofstream out("results.csv");
    out << "N,Op1_Mean_us,Op1_Std_us,Op2_Mean_us,Op2_Std_us,Op3_Mean_us,Op3_Std_us,Memory_MB\n";


    for (size_t N : sizes)
    {
        cout << "Тестування для N = " << N << "\n";
        vector<Student> test_data = generateData(base_data, N);

        vector<double> op1_times, op2_times, op3_times;

        for (int r = 0; r < repetitions; ++r)
        {
            DatabaseVariantC db;
            db.init(test_data);

            auto start = high_resolution_clock::now();
            db.get_top_100();
            auto end = high_resolution_clock::now();
            op1_times.push_back(duration_cast<microseconds>(end - start).count());

            string test_email = test_data[N / 2].m_email;
            start = high_resolution_clock::now();
            for (int k = 0; k < 1000; ++k) {
                db.update_rating(test_email, 50.0f + (k % 50));
            }
            end = high_resolution_clock::now();
            op2_times.push_back(duration_cast<microseconds>(end - start).count() / 1000.0);

            start = high_resolution_clock::now();
            db.get_best_groups();
            end = high_resolution_clock::now();
            op3_times.push_back(duration_cast<microseconds>(end - start).count());
        }

        auto stats1 = calcStats(op1_times);
        auto stats2 = calcStats(op2_times);
        auto stats3 = calcStats(op3_times);

        double memory_mb = (N * 160.0) / (1024.0 * 1024.0);

        out << N << ","
            << stats1.first << "," << stats1.second << ","
            << stats2.first << "," << stats2.second << ","
            << stats3.first << "," << stats3.second << ","
            << memory_mb << "\n";
    }

    out.close();
    cout << "Експеримент завершено. Дані збережено results.csv\n";
    return 0;
}
