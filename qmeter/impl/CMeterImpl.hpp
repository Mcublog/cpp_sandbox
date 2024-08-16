/**
 * @file CMeterImpl.hpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-08-16
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#pragma once

#include <CapacityMeter/CMeter.hpp>
#include <CapacityMeter/battery/Profile.hpp>
#include <CapacityMeter/measurement/IDevice.hpp>

class MockDevice final : public IMeasurementDevice
{
  public:
    MeasermentData mesurment()
    {
        return MeasermentData{.mV = 4200, .mA = 1000};
    }
};

class LiIonDischargeGraph final : public IDischargeGraph
{
  private:
    // 0     2.5
    // 10    3.0
    // 20    3.2
    // 30    3.4
    // 40    3.5
    // 50    3.6
    // 60    3.7
    // 70    3.8
    // 80    3.9
    // 90    4.0
    // 100   4.2
    static constexpr uint8_t kChartSize = 11;
    const uint16_t kChart[kChartSize]{
        2500, 3000, 3200, 3400, 3500, 3600, 3700, 3800, 3900, 4000, 4200,
    };

  public:
    uint8_t procent(uint32_t mV)
    {
        if (mV < kChart[0])
            return 0;
        else if (mV > kChart[kChartSize - 1])
            return 100;

        uint8_t p = 0;
        for (uint8_t i = 0; i < (kChartSize - 1); i++)
        {
            if (mV >= kChart[i] && mV <= kChart[i + 1])
                return (i + 1) * 10;
        }
        return 0;
    }
};