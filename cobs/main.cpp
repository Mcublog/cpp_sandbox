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
#include "libs/framing/framing.hpp"
#include "cobs/sample_enc_dec.hpp"
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

    int r = framing::frame_send(0x33, payload, sizeof(payload), ll_send);
    LOG_INFO("----------");
    r = framing::frame_send(0x34, payload, sizeof(payload) - 100, ll_send);
    LOG_INFO("----------");

    const uint8_t kTestDataIn[] = {
        0x02, 0x34, 0x02, 0x23, 0x01, 0x01, 0x02, 0x01, 0x27, 0x21, 0x43, 0x34,
        0x12, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
        0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x00};

    for (uint32_t i = 0; i < sizeof(kTestDataIn); i++)
    {
        chunk_t *chunk = framing::chunk_receive(kTestDataIn[i]);
        if (chunk == nullptr)
            continue;

        LOG_INFO("chunk rx: channel: 0x%02x size: %d", chunk->channel, chunk->size);
    }
    return 0;
    // return encode_decode();
}