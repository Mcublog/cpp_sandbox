
#ifndef COBS_FRAMING_TYPES_H
#define COBS_FRAMING_TYPES_H

#include <stdint.h>

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

#endif // COBS_FRAMING_TYPES_H