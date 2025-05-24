/**
 * @file main.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-03-22
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include "print.hpp"

//>>---------------------- Log control
#define LOG_MODULE_NAME cmain
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

#define SIZE_ARRAY (32)

int main()
{
    uint8_t a[SIZE_ARRAY] = {};
    LOG_INFO("a[%d]: 0x%x", sizeof(a), &a);

    print_array(a);
    return 0;
}