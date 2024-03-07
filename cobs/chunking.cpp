/**
 * @file chunking.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <cstdint>
#include <cstring>

#include "cobs/chunking.hpp"
#include "libs/nanocobs/cobs.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME chnk
#define LOG_MODULE_LEVEL (3)
#include "common/debug/log_libs.h"
//<<----------------------

//>>---------------------- Locals
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

static constexpr uint8_t kCobsOverHead = 2;
static constexpr uint8_t kChunkBuffer = 128 + sizeof(chunk_t) + kCobsOverHead;
static constexpr uint8_t KChunkHeader = sizeof(chunk_t);
static constexpr uint8_t kChunkPayloadMax = kChunkBuffer - KChunkHeader - kCobsOverHead;

/**
 * @brief Cкопировать данные в payload, выдает кол-во закодированных данных.
 * Выставить границы сообщения.
 *
 * @param buffer размером kChunkBuffer
 * @param offset смещение входных данных
 * @param data
 * @param size
 * @return uint32_t
 */
static uint32_t prepare_chunk_to_inplace_encode(uint8_t *buffer, uint32_t offset,
                                                uint8_t *data, uint32_t size)
{
    chunk_t *chunk = reinterpret_cast<chunk_t *>(&buffer[1]);
    std::memcpy(chunk->payload, &data[offset], size);
    uint32_t outputlen = KChunkHeader + size;
    buffer[0] = COBS_INPLACE_SENTINEL_VALUE;
    buffer[outputlen] = COBS_INPLACE_SENTINEL_VALUE;
    outputlen++;
    return outputlen;
}

/**
 * @brief
 *
 * @param chunk
 * @param channel
 * @param number
 * @param total
 * @param data
 * @param size
 */
static void fill_header(chunk_t *chunk, uint8_t channel, uint16_t number, uint16_t total,
                        uint8_t *data, uint32_t size)
{
    chunk->channel = channel;
    chunk->number = number;
    chunk->total = total;
    // TODO: calc crc32
    chunk->crc = 0x1234'4321;
}

/**
 * @brief
 *
 * @param buffer
 * @param channel
 * @param number
 * @param total
 * @param data
 * @param size, максимальный размер == kChunkPayloadMax
 * @return uint32_t
 */
static uint32_t chunk_encode(uint8_t *buffer, uint8_t channel, uint16_t number, uint16_t total,
                             uint8_t *data, uint32_t size)
{
    chunk_t *chunk = reinterpret_cast<chunk_t*>(&buffer[1]);
    fill_header(chunk, channel, number, total, data, size);
    uint32_t outputlen = prepare_chunk_to_inplace_encode(buffer, 0, data, size);
    cobs_ret_t r = cobs_encode_inplace(buffer, outputlen);

    LOG_INFO("chunk nmbr: %d total: %d", number, total);
    LOG_DEBUG("tx => raw: %d enc: %d", KChunkHeader + size, outputlen);
    return outputlen;
}

//<<----------------------

/**
 * @brief
 *
 * @param channel
 * @param data
 * @param size
 * @return int
 */
uint32_t chunking::send_data(uint8_t channel, uint8_t *data, uint32_t size)
{
    uint8_t buffer[kChunkBuffer] = {};

    const uint32_t kChunks = size < kChunkPayloadMax ? 0 : size / kChunkPayloadMax;
    const uint32_t kTotal = (kChunks + 1);
    const uint32_t kRemainder =
        kChunks ? kChunks * size % kChunkPayloadMax : kTotal * size % kChunkPayloadMax;

    LOG_INFO("chunks: %d rem: %d", kTotal, kRemainder);

    for (uint32_t i = 0, offset = 0; i <= kChunks; i++, offset = (i * kChunkPayloadMax))
    {
        uint32_t size_to_encode = i != kChunks ? kChunkPayloadMax : kRemainder;
        if (size_to_encode == 0)
            continue;
        chunk_encode(buffer, channel, i, kTotal, &data[offset], size_to_encode);
        // TODO: send outputlen
    }

    return 0;
}