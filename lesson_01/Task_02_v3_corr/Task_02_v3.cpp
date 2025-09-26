#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Windows.h"


void fillVector(std::vector<int>& vec1, int size)
{
    for (int i = 0; i < size; ++i)
    {
        vec1.push_back(i);
    }
}

void sumVector(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& vecResult, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        vecResult[i] = vec1[i] + vec2[i];
    }
}

void timeThread(std::vector<double>& timeCount, std::vector<int>& vec1, std::vector<int>& vec2, std::vector<int>& vecResult, int threads)
{
    std::vector<std::thread> fillThreads;
    int n = static_cast<int>(vec1.size() / threads);
    std::vector<int> startEnd{};
    for (int k = 0; k < threads; ++k)
    {
        startEnd.push_back(k * n);
    }
    startEnd.push_back(vec1.size() - 1);
    auto start = std::chrono::high_resolution_clock::now();
    for (int l = 0; l < threads; ++l)
        fillThreads.push_back(std::thread(sumVector, std::ref(vec1), std::ref(vec2), std::ref(vecResult), startEnd[l], startEnd[l + 1] - 1));
    for (auto& thread : fillThreads)
    {
        thread.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto k = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    double u = static_cast<double>(k.count()) / 1000000000;
    
    timeCount.push_back(u);
}

void ptintTable(std::vector<double>& timeCount, std::vector<int>& size, std::vector<int>& threads)
{
    std::cout << "\t\t\t\t\t\tSize of threads" << std::endl;
    std::cout << "\t\t";
    for (auto& i : size)
    {
        std::cout << i << "\t\t";
    }
    std::cout << std::endl;
    std::cout << "Number of threads\t\t\t\tTime,s " << std::endl;
    int j = 0;
    for (int i = 0; i < threads.size(); ++i)
    {
        std::cout << threads[i] << "  threads   ";
        for (int k = 0; k < size.size(); ++k)
        {
            printf("\t%4.4f\t", timeCount[j]);
            ++j;
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "Number of scores: " << std::thread::hardware_concurrency() << std::endl;
    auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::vector<double> timeCount{};
    std::vector<int> size{ 1000, 10000, 100000, 1000000, 10000000 };
    std::vector<int> threads{ 1, 2, 4, 8, 16 };
    for (auto i : threads)
    {
        for (auto j : size)
        {
            std::vector<int> vec1, vec2, vecResult;
            fillVector(vec1, j);
            fillVector(vec2, j);
            fillVector(vecResult, j);
            timeThread(timeCount, vec1, vec2, vecResult, i);
        }
    }
    ptintTable(timeCount, size, threads);
    return 0;
}