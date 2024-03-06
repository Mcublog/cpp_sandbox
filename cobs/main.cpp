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
#include "cobs/chunking.hpp"
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
    uint8_t payload[135] = {};
    for (uint32_t i = 0; i < sizeof(payload); i++)
        payload[i] = i + 1;

    int r = 0;
    r = chunking::send_data(0x33, payload, sizeof(payload));
    LOG_INFO("----------");
    r = chunking::send_data(0x34, payload, sizeof(payload) - 100);
    return 0;
    // return encode_decode();
}