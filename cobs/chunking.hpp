
#ifndef COBS_CHUNKING_HPP
#define COBS_CHUNKING_HPP

#include <cstdint>

namespace chunking
{
uint32_t send_data(uint8_t channel, uint8_t *data, uint32_t size);
}

#endif // COBS_CHUNKING_HPP