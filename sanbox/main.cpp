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

#include "libs/bitarray-cpp/bitarray.h"
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

    bit_array_c bits{line, 640};

    bits.SetBit(4);
    bits.SetBit(9);
    while (1)
    {
        LOG_INFO("waiting");
        sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
