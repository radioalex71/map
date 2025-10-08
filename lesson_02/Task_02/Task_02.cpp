#include "Timer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <string>

std::once_flag flag;
std::mutex mtx1;
std::mutex mtx2;
std::mutex mtx3;
std::mutex mtx4;

static void tableHeader()
{
    std::cout << "#\t" << "Thread ID\t" << "Progress_bar\t\t" << "Time " << std::endl;
}

static void progressBar(int numberThreads)
{
    int progressBar = 23;
    int total = 37;
    int positionTime = 43;

    std::call_once(flag, tableHeader);

    mtx2.lock();
    Timer t1;
    consoleParams p1;
    mtx2.unlock();

    mtx4.lock();
    p1.setPosition(0, numberThreads + 1);
    std::cout << numberThreads << "\t" << std::this_thread::get_id() << "\t";
    mtx4.unlock();

    while (progressBar < total)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        mtx1.lock();
        p1.setPosition(progressBar, numberThreads + 1);
        std::cout << char(219);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        progressBar++;
        mtx1.unlock();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    mtx3.lock();
    p1.setPosition(positionTime, numberThreads + 1);
    t1.print();
    mtx3.unlock();
}

int main()
{
    int countThreads = 5;
    std::vector<std::thread> threads(countThreads);
    for (int i = 0; i < countThreads; i++)
    {
        threads[i] = std::thread(progressBar, i);
    }
    for (auto& th : threads)
    {
        th.join();
    }
    std::cout << "\n\n";
    return 0;
}