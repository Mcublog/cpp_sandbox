/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-07-16
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include "main.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmain
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------
int main(void)
{
    LOG_INFO("main call");

    return 0;
}
