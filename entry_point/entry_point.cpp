/**
 * @file static_allocator.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <cstdlib>

#include "entry_point.h"

#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME entry
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

int entry_point(void)
{
    LOG_INFO("entry_point call");

    exit(main());
}