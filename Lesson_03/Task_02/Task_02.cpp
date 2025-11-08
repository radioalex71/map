#include <iostream>
#include <thread>
#include <list>
#include <iterator>
#include <algorithm>
#include <future>
#include <vector>
//#include "Windows.h"

typedef unsigned int ui;

int increment(int value)
{
    return value + 1;
}

template <typename It, typename Func>
void async_for_each(It first, It last, Func func)
{
    const ui length = std::distance(first, last);
    if (!length)
        return;
    const ui min_per_thread = 25;
    if (length < (2 * min_per_thread))
    {
        std::for_each(first, last, func);
    }
    else
    {
        It const mid_point = first + length / 2;
        std::future<void> first_half = std::async(&async_for_each<It, Func>, first, mid_point, func);
        std::future<void> second_half = std::async(&async_for_each<It, Func>, mid_point, last, func);
        first_half.get();
        second_half.get();
    }
}

void printVector(std::vector<int>& vec)
{
    for (const auto& elem : vec)
        std::cout << elem << " ";
    std::cout << std::endl;
}

int main()
{
    std::vector<int> vec = { 11, 22, 3, 4, 55, 6, 7, 8, 9, 10 };
    printVector(vec);
    async_for_each(vec.begin(), vec.end(), [](int& elem) { elem = increment(elem); });
}