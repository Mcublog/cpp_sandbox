/**
 * @file print.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-05-23
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include "print.hpp"

//>>---------------------- Log control
#define LOG_MODULE_NAME print
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

/**
 * @brief
 *
 * @param data
 */
void print_array(std::span<uint8_t> data)
{
    LOG_INFO("a[%lu]: from adr: 0x%x", data.size(), data.data());
    for(size_t i = 0; i < data.size(); i++)
    {
        if (i != 0 && i % 16 == 0)
            LOG_RAW_INFO("\n");
        LOG_RAW_INFO("0x%02x ", data[i]);
    }
    LOG_RAW_INFO("\n");
    LOG_RAW_INFO("-----------------------------------------------------------------------"
                 "--------\n");
}