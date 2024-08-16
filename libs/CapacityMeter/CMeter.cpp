/**
 * @file CMeter.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-08-16
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "CMeter.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME     cmeter
#define LOG_MODULE_LEVEL    (3)
#include "libs/debug/log_libs.h"
//<<----------------------

void CMeter::process()
{
    MeasermentData d = dev.mesurment();
    LOG_INFO("%d mV %d mA", d.mV, d.mV);
}