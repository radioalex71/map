#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;

int maxClients = 10;
std::atomic<int> clientCount = 0;
//int clientCount = 0;

void clientThread()
{
    while (clientCount < maxClients)
    {
        clientCount.fetch_add(1, std::memory_order_seq_cst);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //clientCount++;
        std::cout << "Number of clients to serve: " << clientCount + 1 << std::endl;
    }
}

void operatorThread()
{
    while (true)
    {
        clientCount.fetch_sub(1, std::memory_order_seq_cst);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (clientCount > 0)
        {
            //clientCount--;
            std::cout << "Clients in the queue: " << clientCount - 1 << std::endl;
        }
        else
        {
            std::cout << "Operator is finished!" << std::endl;
            break;
        }
    }
}

int main()
{
    std::thread t1(clientThread);
    std::thread t2(operatorThread);

    t1.join();
    t2.join();

    return 0;
}