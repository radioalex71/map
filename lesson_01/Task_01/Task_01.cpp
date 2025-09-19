#include <iostream>
#include <thread>
#include <chrono>

int maxClients = 10;
int clientCount = 0;

void clientThread()
{
    while (clientCount < maxClients)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        clientCount++;
        std::cout << "Number of clients to serve: " << clientCount << std::endl;
    }
}

void operatorThread()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        if (clientCount > 0)
        {
            clientCount--;
            std::cout << "Clients in the queue: " << clientCount << std::endl;
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