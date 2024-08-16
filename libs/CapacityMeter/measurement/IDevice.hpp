#pragma once

#include <cstdint>

struct MeasermentData
{
    int16_t mV;
    int16_t mA;
};

/**
 * @brief Измеритель параметров батареи
 * Ток
 * Напряжение
 *
 */
class IMeasurementDevice
{
  public:
    virtual MeasermentData mesurment() = 0;
};
