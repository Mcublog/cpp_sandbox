/**
 * @file main.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-08-06
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

void rts_set(int fd, bool on)
{
    int status = 0;
    // Отключаем RTS
    if (ioctl(fd, TIOCMGET, &status) == -1)
    {
        perror("Unable to get current serial line status");
        close(fd);
        return;
    }

    if (!on)
        status &= ~TIOCM_RTS; // Сбрасываем бит RTS
    else
        status |= TIOCM_RTS; // Сбрасываем бит RTS

    if (ioctl(fd, TIOCMSET, &status) == -1)
    {
        perror("Unable to set serial line status");
        close(fd);
        return;
    }
}

void number_bytes_in_buffer(int fd)
{
    int bytes_in_buffer = 0;

    // Используем ioctl для получения количества байтов в буфере
    if (ioctl(fd, FIONREAD, &bytes_in_buffer) == -1)
    {
        perror("Unable to get the number of bytes in the buffer");
        close(fd);
        return;
    }

    printf("Bytes in receive buffer: %d\n", bytes_in_buffer);
}

int main(void)
{
    printf("start flowcontrol test\n");

    struct termios options;

    // Открываем последовательное устройство
    int fd = open("/dev/ttyS10", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        perror("Unable to open /dev/ttyS10");
        return -1;
    }

    // Получаем текущие настройки устройства
    if (tcgetattr(fd, &options) == -1)
    {
        perror("Unable to get terminal attributes");
        close(fd);
        return -1;
    }

    // Устанавливаем флаг CRTSCTS для аппаратного flow control
    options.c_cflag |= CRTSCTS;

    // Применяем настройки к устройству
    if (tcsetattr(fd, TCSANOW, &options) == -1)
    {
        perror("Unable to set terminal attributes");
        close(fd);
        return -1;
    }

    printf("Hardware flow control (RTS/CTS) has been set on /dev/ttyS10\n");

    //rts_set(fd, false);
    while (true)
    {
        number_bytes_in_buffer(fd);
        sleep(1);
    }

    // Закрываем устройство
    close(fd);
    return 0;
}