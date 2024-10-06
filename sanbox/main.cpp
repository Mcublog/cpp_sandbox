/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-08-16
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <chrono>
#include <thread>

#include "bitarray.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME     sandbox
#define LOG_MODULE_LEVEL    (3)
#include "libs/debug/log_libs.h"
//<<----------------------


int main(void)
{
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono;      // nanoseconds, system_clock, seconds

    LOG_INFO("hello");

    uint8_t line[640 / 8] = {};

    bitarray bits{line, 640};

    bits.set_bit(4);
    bits.set_bit(9);
    bits.reverse();
    while (1)
    {
        LOG_INFO("waiting");
        sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
