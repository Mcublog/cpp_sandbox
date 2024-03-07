
#ifndef COBS_FRAMING_HPP
#define COBS_FRAMING_HPP

#include <cstdint>

typedef struct
{
    uint8_t channel;
    uint8_t *data;
    uint32_t size;
} frame_t;

typedef void(*ll_send_data_t)(uint8_t *data, uint32_t size);

namespace framing
{
void init();
uint32_t send_data(uint8_t channel, uint8_t *data, uint32_t size, ll_send_data_t sender);
bool recv_chunk(uint8_t byte, frame_t *frame);
}

#endif // COBS_FRAMING_HPP