#pragma once

#include <cstdint>

class IDischargeGraph
{
    public:
        virtual uint8_t procent(uint32_t mV) = 0;
};


/**
 * @brief Профиль батареи
 * Сnom Номинальной емкостью
 * Umin/max максимальное/минимальное напряжение
 * Imax/min максимальный/минимальный ток
 * Кривую напряжения соответсвующую емкости
 */
struct Profile
{
    uint32_t Cnom;
    uint32_t Umin;
    uint32_t Umax;
    uint32_t Imax;
    uint32_t Imin;

    IDischargeGraph &graph;
};
