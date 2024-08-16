#pragma once

#include <CapacityMeter/Battery.hpp>
#include <CapacityMeter/measurement/IDevice.hpp>

/**
 * @brief Алгоритм подсчета емкости
 * 1. Если ток не идет смотрим напряжение
 * 2. Раз в 100 мс опрашиваем ток и напряжение
 * 3. Сохраняем ток разряда и таким образом получаем оставшейся процент заряда
 * 4. Также фиксируем ток заряда
 * 5. По току разряда/заряда корректируем фактическую емкость
 */
class CMeter
{
  private:
    Battery battery;
    IMeasurementDevice &dev;

  public:
    CMeter(Profile &profile, IMeasurementDevice &dev) : battery{profile}, dev{dev} {};
    uint8_t procent() const;

    void process();
};
