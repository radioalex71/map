#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class Data
{
public:

    int data_ = 0;
    std::mutex mtx_;
    Data() {};
    Data(int data)
    {
        data_ = data;
    }
    void printData()
    {
        std::cout << "data_ = " << data_ << " " << std::endl;
    }
};
void swapLock(Data& data1, Data& data2)
{
    if (&data1 == &data2)\
    {
        return;
    }

    std::lock(data1.mtx_, data2.mtx_);
    std::lock_guard<std::mutex> lock1(data1.mtx_, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(data2.mtx_, std::adopt_lock);
    std::swap(data1.data_, data2.data_);
    data1.printData();
    data2.printData();
    std::cout << std::endl;
}

void swapScopedLock(Data& data1, Data& data2)
{
    if (&data1 == &data2)
    {
        return;
    }
    std::scoped_lock lock(data1.mtx_, data2.mtx_);
    std::swap(data1.data_, data2.data_);
    data1.printData();
    data2.printData();
    std::cout << std::endl;
}

void swapUniqueLock(Data& data1, Data& data2)
{
    if (&data1 == &data2)
    {
        return;
    }
    std::unique_lock<std::mutex> lock1(data1.mtx_, std::defer_lock);
    std::unique_lock<std::mutex> lock2(data2.mtx_, std::defer_lock);
    std::lock(lock1, lock2);
    std::swap(data1.data_, data2.data_);
    data1.printData();
    data2.printData();
    std::cout << std::endl;
}
int main()
{
    Data data1(10);
    Data data2(20);
    auto print = [&data1, &data2]()
        {
            data1.printData();
            data2.printData();
            std::cout << std::endl;
        };
    print();
    std::thread t1(swapLock, std::ref(data1), std::ref(data2));
    std::thread t2(swapScopedLock, std::ref(data1), std::ref(data2));
    std::thread t3(swapUniqueLock, std::ref(data1), std::ref(data2));

    t1.join();
    t2.join();
    t3.join();
    return 0;
}