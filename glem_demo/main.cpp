#include <glem.h>

//>>---------------------- Log control
#define LOG_MODULE_NAME glemdemo
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

uint8_t buffer[128 / 8 * 64] = {0};

int main()
{
    LOG_INFO("glem demo");
    glem_init(128, 64, 0);
    for (uint16_t i = 0; i < sizeof(buffer); i++)
        buffer[i] = i & 0xFF;
    glem_write(buffer, sizeof(buffer));

    return 0;
}