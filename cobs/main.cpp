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
#include "cobs/framing.hpp"
#include "cobs/sample_enc_dec.hpp"
#include "libs/nanocobs/cobs.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME dcobs
#define LOG_MODULE_LEVEL (4)
#include "common/debug/log_libs.h"
//<<----------------------

void ll_send(uint8_t *data, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
        LOG_RAW_DEBUG("%02X ", data[i]);
    LOG_RAW_DEBUG("\r\n");
}

/**
 * @brief
 *
 */
int main(void)
{
    uint8_t payload[135] = {};
    for (uint32_t i = 0; i < sizeof(payload); i++)
        payload[i] = i + 1;

    framing::init();

    int r = 0;
    r = framing::send_data(0x33, payload, sizeof(payload), ll_send);
    LOG_INFO("----------");
    r = framing::send_data(0x34, payload, sizeof(payload) - 100, ll_send);
    LOG_INFO("----------");

    frame_t frame = {};
    bool ret = framing::recv_chunk(0xAA, &frame);
    return 0;
    // return encode_decode();
}