/**
 * @file wrapper.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief C wrapper
 * @version 0.1
 * @date 2024-03-07
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include "framing.h"
#include "framing.hpp"
#include "types.h"

void fr_init(void)
{
    framing::init();
}

uint32_t fr_frame_send(uint8_t channel, uint8_t *data, uint32_t size,
                       ll_send_data_t sender)
{
    return framing::frame_send(channel, data, size, sender);
}

chunk_t *fr_chunk_receive(uint8_t data_byte)
{
     return framing::chunk_receive(data_byte);
}
