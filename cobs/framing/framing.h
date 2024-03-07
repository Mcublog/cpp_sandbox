
#ifndef COBS_FRAMING_H
#define COBS_FRAMING_H

#include <stdint.h>

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void fr_init(void);
    uint32_t fr_frame_send(uint8_t channel, uint8_t *data, uint32_t size,
                           ll_send_data_t sender);
    chunk_t *fr_chunk_receive(uint8_t data_byte);

#ifdef __cplusplus
}
#endif

#endif // COBS_FRAMING_H