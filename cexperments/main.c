/**
 * @file main.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2026-03-01
 *
 * @copyright Mcublog Copyright (c) 2026
 *
 */
#include "obj.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmain
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------
int main()
{
    uint8_t buffer[OBJ_SIZE] = {};
    obj *o = obj_placement_new(buffer, sizeof(buffer));

    LOG_INFO("mr: 0x%x", obj_mark(o));

    return 0;
}