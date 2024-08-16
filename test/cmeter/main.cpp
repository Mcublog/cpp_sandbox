/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief CMeter some tests
 * @version 0.1
 * @date 2024-08-16
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "qmeter/impl/CMeterImpl.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmeter_tst
#define LOG_MODULE_LEVEL (3)
#ifdef NDEBUG
#undef NDEBUG
#endif
#include "libs/debug/log_libs.h"
//<<----------------------

/**
 * @brief Проверка графика разрядки по напряжению для LiIon батареи
 *
 * @return int
 */
int li_ion_chart_test()
{
    LiIonDischargeGraph chart;

    const uint16_t kVoltages[] = {1200, 2500, 2600, 3400, 3850, 4100, 4200, 4300};

    static constexpr uint8_t kSize = 8;
    const uint8_t kReference[kSize] = {0, 10, 10, 30, 80, 100, 100, 100};
    uint8_t results[kSize] = {};

    for (uint8_t i = 0; i < sizeof(kVoltages) / sizeof(kVoltages[0]); i++)
    {
        uint8_t proc = chart.procent(kVoltages[i]);
        results[i] = proc;
        LOG_INFO("%d mv == %d %%", kVoltages[i], proc);
    }

    for (auto i = 0; i < kSize; i++)
    {
        if (results[i] == kReference[i])
            continue;
        LOG_ERROR("results[%d]: %d != kReference[%d]: %d", i, results[i], i, kReference[i]);
        return -1;
    }

    return 0;
}

int main(void)
{
    int err = li_ion_chart_test();
    if (err)
    {
        LOG_ERROR("li_ion_chart_test ... FAILED");
    }

    return 0;
}