#pragma once
#include <chrono>
#include <iostream>
#include "Windows.h"
#include <thread>

class consoleParams
{
public:
    static void setColore(int text, int background)
    {
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }
    static void setPosition(int x, int y)
    {
        COORD point;
        point.X = x;
        point.Y = y;
        SetConsoleCursorPosition(hStdOut, point);
    }
private:
    static HANDLE hStdOut;
};
HANDLE consoleParams::hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

class Timer
{
public:
    std::string mName;
    Timer() : m_beg(clock_t::now()) {}
    Timer(std::string name) : mName(name), m_beg(clock_t::now()) {}
    void start(std::string name)
    {
        name = mName;
        m_beg = clock_t::now();
    }
    void print() const
    {
        std::cout << "ID" << std::this_thread::get_id() << " " << mName << " " << elapsed() * 1000 << " ms" << std::endl;
    }


private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;
    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
};