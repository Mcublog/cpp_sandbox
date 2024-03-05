/**
 * @file main.cpp
 * @author Viacheslav (slava.k@ks2corp.com)
 * @brief
 * @version 0.1
 * @date 2024-03-04
 *
 * @copyright KS2 Copyright (c) 2024
 *
 */

#include "libs/nanocobs/cobs.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME dcobs
#define LOG_MODULE_LEVEL (3)
#include "common/debug/log_libs.h"
//<<----------------------

typedef struct
{
    uint8_t channel;
    uint8_t reserved[7];
} header_t;

void *get_header_from_somewhere(size_t *s)
{
    static header_t test_head = {
        .channel = 0x11
    };

    *s = sizeof(test_head);
    return &test_head;
}

void* get_payload_from_somewhere(size_t *s)
{
    static uint8_t payload[] = {0, 1, 2, 3, 4, 5, 6};
    *s = sizeof(payload);
    return payload;
}

/**
 * @brief
 *
 */
int main(void)
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

    size_t encoded_len;
    r = cobs_encode_inc_end(&ctx, &encoded_len);
    if (r != COBS_RET_SUCCESS)
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }

    /* At this point, |encoded| contains the encoded header and payload.
       |encoded_len| contains the length of the encoded buffer. */

    char decoded[128];
    size_t decoded_len = 0;
    cobs_ret_t const result = cobs_decode(encoded, encoded_len, decoded, sizeof(decoded), &decoded_len);
    if (result == COBS_RET_SUCCESS)
    {
        LOG_INFO("result == COBS_RET_SUCCESS");
    }
    else
    {
        LOG_ERROR("r != COBS_RET_SUCCESS");
        return -1;
    }

    return 0;
}