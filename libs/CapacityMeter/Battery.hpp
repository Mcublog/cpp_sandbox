#pragma once

#include <cstdint>

#include <CapacityMeter/battery/Profile.hpp>

/**
 * @brief Фактическая батарея
 *
 */
typedef enum
{
    IDLE,
    CHARGE,
    DISCHARGE
} charge_status_t;

class Battery
{
  private:
    uint32_t capacity; // фактическая емкость
    uint32_t cycles;   // количество зарядов/разрядов
    charge_status_t status;

    Profile &profile;

  public:
    Battery(Profile &profile): profile{profile} {};
};