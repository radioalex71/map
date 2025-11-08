#include <iostream>
#include <algorithm>
#include <future>
#include <vector>
#include <random>

void sort(std::vector<int>& v)
{
    auto n = v.size();
    for (int i = 0; i < n - 1; i++)

    {
        int minElement = v[i];
        for (int j = i + 1; j < n; j++)
        {
            if ((v[j] < minElement) && (minElement != v[j]))
            {
                minElement = v[j];
                std::swap(v[i], v[j]);
            }
        }
    }
}

void findMin(const std::vector<int> v, int i, int vSize, std::promise<int> p)
{
    int minIndex = i;
    for (int j = i + 1; j < vSize; j++)
    {
        if (v[j] < v[minIndex])
        {
            minIndex = j;
        }
    }
    p.set_value(minIndex);
}

void printVector(const std::vector<int>& v)
{
    for (const auto& i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> v = { 8, 4, 3, 2, 5, 6, 7, 1, 9, 10 };
    int vecSize = v.size();
    std::cout << "Original vector: ";
    printVector(std::ref(v));
    for (int i = 0; i < vecSize - 1; i++)
    {
        std::promise<int> p;
        std::future<int> f = p.get_future();
        auto asyncFindMin = std::async(std::launch::async, findMin, std::ref(v), i, vecSize, std::move(p));
        int minIndex = f.get();
        if (v[i] != v[minIndex])
        {
            std::swap(v[i], v[minIndex]);
        }
    }
    std::cout << "Sorted vector: ";
    printVector(std::ref(v));

    return 0;
}