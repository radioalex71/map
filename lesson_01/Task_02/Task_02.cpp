#include <iostream>
#include "Windows.h"
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <execution>

void fillArray(std::vector<int> arr, int size) //функция заполнения массива
{
    for (int i = 0; i < size; i++)
    {
        arr.push_back(i);
    }
}

void sumArray(std::vector<int> a, std::vector<int> b, int start, int end) //функция суммирования
{
    for (int i = start; i < end; i++)
    int c = a[i] + b[i];
}

//функция посчета времени
void time_thread(std::vector<double>& timeCount, int count, std::vector<int> a, std::vector<int> b, int threads)
{
    std::vector<std::thread> fillThreads;
    int n = static_cast<int>(a.size() / threads);
    std::vector<int> start_end{0};
    for (int k = 0; k < threads - 1; k++)
    {
        start_end.push_back(k * n);
    }
    start_end.push_back(a.size() - 1);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threads; i++)
    {
        fillThreads.push_back(std::thread(sumArray, a, b, start_end[i], start_end[i + 1] - 1));
    }
    for (auto& t : fillThreads)
    {
        t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto k = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

    double l = static_cast<double> (k.count() / 1000000);
    timeCount.push_back(l);
}
//функция для вывода таблицы результатов
void print_table(std::vector<double>& timeCount, std::vector<int>threads, std::vector<int> size)
{
    std::cout << " \t\t\t\t\t\t Размер массивов для суммирования" << std::endl;
    std::cout << "\t\t\t  ";
    for (auto i : size)
    {
        std::cout << "" << i << "\t\t";
    }
    std::cout << std::endl << "Количество потоков \t\t\t\t время работы в мс" << std::endl;
    int j = 0;
    for (int i = 0; i < threads.size(); i++)
    {
        std::cout << "   " << threads[i] << "      Поток";
        for (int f = 0; f < size.size(); f++)
        {
            printf("  \t%4.4f", timeCount[j]);
            j++;
        }
        std::cout << std::endl;
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Количество аппаратных ядер: " << std::thread::hardware_concurrency() << std::endl;
    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::vector<double> timeCount;
    std::vector<int> size{ 1000, 10000, 100000, 1000000, 10000000 };
    std::vector<int>threads{ 1, 2, 4, 8, 16 };
    for (auto i : threads)
    {
        for (auto j : size)
        {
            std::vector<int> a, b;
            fillArray(a, j);
            fillArray(b, j);
            time_thread(timeCount, j, a, b, i);
        }
    }
    print_table(timeCount, threads, size);
    return 0;
}