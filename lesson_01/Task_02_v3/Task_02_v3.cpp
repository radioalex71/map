#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Windows.h"

static void sumVectors(double& sumOfVectors, const std::vector<int> newVector1, const std::vector<int> newVector2)
{
    for (int i = 0; i < newVector1.size(); ++i)
    {
        sumOfVectors += newVector1.at(i) + newVector2.at(i);
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Количество аппаратных ядер: " << std::thread::hardware_concurrency() << "\n\n";
    std::cout << std::setw(9) << "\t\t   1'000" << "\t    10'000" << "\t    100'000" << "\t   1'000'000" << "\n";
    std::cout << "-----------------------------------------------------------------------------" << std::endl;
    // Создаем два вектора
    std::vector<int> vector1;
    std::vector<int> vector2;
    // Вектор количества потоков
    std::vector<int> numOfThreads{ 1, 2, 4, 8, 16 };
    // Вектор для размера векторов
    std::vector<int> sizeOfVector{ 1'000, 10'000, 100'000, 1'000'000 };

    for (auto& countT : numOfThreads)
    {
        if (countT == 1)
        {
            std::cout << std::endl << countT << "     поток";
        }
        else
        {
            std::cout << std::endl << countT << " потока(ов)";
        }

        for (auto& countV : sizeOfVector)
        {
            vector1.resize(countV, 2);
            vector2.resize(countV, 5);
            std::vector<std::thread> threads;
            double sumOfVectors = 0;

            int partOfTheSize = static_cast<int> (countV / countT);

            auto thrStart = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < countT; i++)
            {
                std::vector<int> newVector1;
                std::vector<int> newVector2;
                int rightBorder = 0;

                if (i != countT - 1)
                {
                    rightBorder = partOfTheSize * (i + 1);
                }
                else
                {
                    rightBorder = countV;
                }

                for (int j = partOfTheSize * i; j < rightBorder; ++j)
                {
                    newVector1.push_back(vector1.at(j));
                    newVector2.push_back(vector2.at(j));
                }
                //sumVectors(std::ref(sumOfVectors), std::ref(newVector1), std::ref(newVector2));
                threads.push_back(std::thread(sumVectors, std::ref(sumOfVectors), newVector1, newVector2));
            }
            for (auto& it : threads)
            {
                it.join();
            }

            auto thrStop = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> thrTime = thrStop - thrStart;
            std::cout << std::setw(14) << /*"\t" <<*/ thrTime.count() << " мс";
        }
        std::cout << "\n\n";
    }
    return 0;
}