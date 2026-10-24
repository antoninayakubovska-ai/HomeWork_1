#include "student.h"
#include "task1_3.cpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <cmath>
#include <numeric>
#include <random>
#include <unordered_set>
#include <malloc.h>

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
    double mean = accumulate(times.begin(), times.end(), 0.0) / times.size();
    double sq = 0;
    for (double t : times) sq += (t - mean) * (t - mean);
    double stdev = sqrt(sq / (times.size() - 1)); 
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

    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 20000, 40000, 60000, 80000, 100000};
    int repetitions = 7;
    const int K1 = 300, K2 = 20000, K3 = 300;

    mt19937 rng(7);

    ofstream out("results.csv");
    out << "N,G,Op1_Mean_us,Op1_Std_us,Op2_Mean_us,Op2_Std_us,Op3_Mean_us,Op3_Std_us,Memory_MB\n";

    for (size_t N : sizes)
    {
        cout << "Тестування для N = " << N << "\n";
        vector<Student> test_data = generateData(base_data, N);

        unordered_set<string> groups;
        for (const auto& s : test_data) groups.insert(s.m_group);

        uniform_int_distribution<size_t> pick(0, N - 1);
        uniform_real_distribution<float> rnd(0.0f, 100.0f);
        vector<size_t> idx(K2);
        vector<float> rating(K2);
        for (int i = 0; i < K2; ++i) { idx[i] = pick(rng); rating[i] = rnd(rng); }

        vector<double> op1_times, op2_times, op3_times, mem_bytes;
        size_t sink = 0;

        for (int r = 0; r < repetitions; ++r)
        {
            malloc_trim(0);
            size_t before = mallinfo2().uordblks;
            DatabaseVariantC* db = new DatabaseVariantC();
            db->init(test_data);
            size_t after = mallinfo2().uordblks;
            mem_bytes.push_back((double)(after - before));

            for (int i = 0; i < 20; ++i) { db->get_top_100(); db->get_best_groups(); }

            //середній час одного виклику
            auto start = high_resolution_clock::now();
            for (int i = 0; i < K1; ++i) sink += db->get_top_100().size();
            auto end = high_resolution_clock::now();
            op1_times.push_back(duration<double, micro>(end - start).count() / K1);

            //оновлення випадкових студентів
            start = high_resolution_clock::now();
            for (int i = 0; i < K2; ++i) sink += db->update_rating(test_data[idx[i]].m_email, rating[i]);
            end = high_resolution_clock::now();
            op2_times.push_back(duration<double, micro>(end - start).count() / K2);

            //середній час одного виклику
            start = high_resolution_clock::now();
            for (int i = 0; i < K3; ++i) sink += db->get_best_groups().size();
            end = high_resolution_clock::now();
            op3_times.push_back(duration<double, micro>(end - start).count() / K3);

            delete db;
        }
        if (sink == 42) cerr << "";

        auto stats1 = calcStats(op1_times);
        auto stats2 = calcStats(op2_times);
        auto stats3 = calcStats(op3_times);
        double memory_mb = calcStats(mem_bytes).first / (1024.0 * 1024.0);

        out << N << "," << groups.size() << ","
            << stats1.first << "," << stats1.second << ","
            << stats2.first << "," << stats2.second << ","
            << stats3.first << "," << stats3.second << ","
            << memory_mb << "\n";
    }

    out.close();
    cout << "Експеримент завершено. Дані збережено results.csv\n";
    return 0;
}
