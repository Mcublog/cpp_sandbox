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

#include "qmeter/impl/CMeterImpl.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmain
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------


int main(void)
{
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono;      // nanoseconds, system_clock, seconds

    LOG_INFO("hello");
    LiIonDischargeGraph chart;

    const uint16_t kVoltages[] = {1200, 2500, 2600, 3400, 3850, 4100, 4200, 4300};
    for (uint8_t i = 0; i < sizeof(kVoltages) / sizeof(kVoltages[0]); i++)
    {
        uint8_t proc = chart.procent(kVoltages[i]);
        LOG_INFO("%d mv == %d %%", kVoltages[i], proc);
    }

    Profile li_ion{.Cnom = 2500,
                   .Umin = 2700,
                   .Umax = 4300,
                   .Imax = 2000,
                   .Imin = 1,
                   .graph = static_cast<IDischargeGraph &>(chart)};

    MockDevice dev;

    CMeter meter{li_ion, static_cast<IMeasurementDevice &>(dev)};

    while (1)
    {
        meter.process();
        sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
