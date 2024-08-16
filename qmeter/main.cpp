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
//>>---------------------- Log control
#define LOG_MODULE_NAME     qmeter
#define LOG_MODULE_LEVEL    (4)
#include "libs/debug/log_libs.h"
//<<----------------------

/**
 * @brief Профиль батареи
 * Сnom Номинальной емкостью
 * Umin/max максимальное/минимальное напряжение
 * Imax/min максимальный/минимальный ток
 * Кривую напряжения соответсвующую емкости
 */
struct BatteryProfile
{
    const uint16_t kIMult = 1000; // множетель тока
    const uint16_t kUMult = 1000; // множетель напряжения

    uint32_t Cnom;
    uint32_t Umin;
    uint32_t Umax;
    uint32_t Imax;
    uint32_t Imin;
    // --- Нужна отдельная сущность
    uint16_t CU_graph[64];
};

/**
 * @brief Фактическая батарея
 *
 */
struct Battery
{
    uint32_t Cact; // фактическая емкость
    uint32_t cycles; // количество зарядов/разрядов
    BatteryProfile profile;
};

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
class MeasurementDevice
{
  private:
    uint16_t period_ms;

  public:
    MeasermentData mesurment();
};

/**
 * @brief
 *
 */
class QMeter
{
  private:
    Battery battery;

  public:
    uint8_t procent() const;
};

/**
 * @brief Алгоритм подсчета емкости
 * 1. Если ток не идет смотрим напряжение
 * 2. Раз в 100 мс опрашиваем ток и напряжение
 * 3. Сохраняем ток разряда и таким образом получаем оставшейся процент заряда
 * 4. Также фиксируем ток заряда
 * 5. По току разряда/заряда корректируем фактическую емкость
 */
int main(void)
{
    LOG_INFO("hello");
    return 0;
}