
#ifndef COBS_FRAMING_HPP
#define COBS_FRAMING_HPP

#include <cstdint>

#include "types.h"

namespace framing
{
void init();
uint32_t frame_send(uint8_t channel, uint8_t *data, uint32_t size, ll_send_data_t sender);
chunk_t *chunk_receive(uint8_t data_byte);
}

#endif // COBS_FRAMING_HPP