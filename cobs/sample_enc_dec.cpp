/**
 * @file sample_enc_dec.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-03-05
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <cstdint>

#include "cobs/sample_enc_dec.hpp"
#include "libs/nanocobs/cobs.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME sencdec
#define LOG_MODULE_LEVEL (3)
#include "log_libs.h"
//<<----------------------

//>>---------------------- Private
typedef struct
{
    uint8_t channel;
    uint8_t reserved[7];
} header_t;

static void *get_header_from_somewhere(size_t *s)
{
    static header_t test_head = {
        .channel = 0x33
    };

    *s = sizeof(test_head);
    return &test_head;
}

static void* get_payload_from_somewhere(size_t *s)
{
    static uint8_t payload[] = {0, 1, 2, 3, 4, 5, 6};
    *s = sizeof(payload);
    return payload;
}

static void printf_buffer(const char *name, uint8_t *buf, size_t len)
{
    LOG_RAW_INFO("%s: ", name);
    for (uint8_t i = 0; i < len; i++)
    {
        LOG_RAW_INFO("%02X", buf[i]);
    }
    LOG_RAW_INFO("\r\n");

}

//<<----------------------

int encode_decode()
{
    cobs_enc_ctx_t ctx;
    char encoded[128] = {0};

    cobs_ret_t r = cobs_encode_inc_begin(encoded, 128, &ctx);
    if (r != COBS_RET_SUCCESS)
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }

    size_t header_len = 0;
    char *header = (char*)get_header_from_somewhere(&header_len);

    r = cobs_encode_inc(&ctx, header, header_len); // encode the header
    if (r != COBS_RET_SUCCESS)
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }

    size_t payload_len;
    char *payload = (char*)get_payload_from_somewhere(&payload_len);
    r = cobs_encode_inc(&ctx, payload, payload_len); // encode the payload
    if (r != COBS_RET_SUCCESS)
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }

    unsigned int encoded_len;
    r = cobs_encode_inc_end(&ctx, &encoded_len);
    if (r != COBS_RET_SUCCESS)
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }

    printf_buffer("encoded", (uint8_t*)encoded, encoded_len);

    /* At this point, |encoded| contains the encoded header and payload.
       |encoded_len| contains the length of the encoded buffer. */

    char decoded[128];
    size_t decoded_len = 0;
    cobs_ret_t result = cobs_decode_inplace(&encoded, encoded_len);
    //  = cobs_decode(encoded, encoded_len, decoded, sizeof(decoded), &decoded_len);
    if (result != COBS_RET_SUCCESS)
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }
    printf_buffer("decoded", (uint8_t*)encoded, encoded_len);

    return 0;
}