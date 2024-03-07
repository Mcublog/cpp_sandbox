
#ifndef COBS_FRAMING_HPP
#define COBS_FRAMING_HPP

#include <cstdint>


typedef struct
{
    uint8_t     channel;     // логический номер канала
    uint8_t     reserved[1]; // резерв
    uint16_t    size;        // количестов данных в чанке
    uint16_t    number;      // номер чанка
    uint16_t    total;       // всего чанков
    uint32_t    crc;         // контрольная сумма payload
    uint8_t     payload[];   // данные
} chunk_t;

typedef void(*ll_send_data_t)(uint8_t *data, uint32_t size);

namespace framing
{
void init();
uint32_t frame_send(uint8_t channel, uint8_t *data, uint32_t size, ll_send_data_t sender);
chunk_t *chunk_receive(uint8_t data_byte);
}

#endif // COBS_FRAMING_HPP