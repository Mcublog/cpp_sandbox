/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "cobs/sample_enc_dec.hpp"
#include "libs/nanocobs/cobs.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME dcobs
#define LOG_MODULE_LEVEL (3)
#include "common/debug/log_libs.h"
//<<----------------------

/**
 * @brief
 *
 */
int main(void)
{
    return encode_decode();
}