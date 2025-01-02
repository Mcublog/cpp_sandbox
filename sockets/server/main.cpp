/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-01-02
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */

#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

//>>---------------------- Log control
#define LOG_MODULE_NAME serv
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

static const char *kSocketPath = "mysocket";
static constexpr int nIncomingConnections = 5;

int main(void)
{
    LOG_INFO("server start");

    // create server side
    int s = 0;
    int s2 = 0;
    struct sockaddr_un local, remote;

    s = socket(AF_UNIX, SOCK_STREAM, 0);
    if (s == -1)
    {
        LOG_ERROR("Error on socket() call");
        return 1;
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, kSocketPath);
    unlink(local.sun_path);

    int err = bind(s, (struct sockaddr *)&local, sizeof(local));
    if (err != 0)
    {
        LOG_ERROR("Error on binding socket");
        return 1;
    }

    if (listen(s, nIncomingConnections) != 0)
    {
        LOG_ERROR("Error on listen call");
    }

    bool bWaiting = true;
    while (bWaiting)
    {
        unsigned int sock_len = 0;
        LOG_ERROR("Waiting for connection....");
        if ((s2 = accept(s, (struct sockaddr *)&remote, &sock_len)) == -1)
        {
            LOG_ERROR("Error on accept() call");
            return 1;
        }

        LOG_ERROR("Server connected");

        int data_recv = 0;
        char recv_buf[100] = {};
        char send_buf[200] = {};
        do
        {
            memset(recv_buf, 0, 100 * sizeof(char));
            memset(send_buf, 0, 200 * sizeof(char));
            data_recv = recv(s2, recv_buf, 100, 0);
            if (data_recv > 0)
            {
                LOG_ERROR("Data received: %d : %s", data_recv, recv_buf);
                strcpy(send_buf, "Got message: ");
                strcat(send_buf, recv_buf);

                if (strstr(recv_buf, "quit") != 0)
                {
                    LOG_ERROR("Exit command received -> quitting");
                    bWaiting = false;
                    break;
                }

                if (send(s2, send_buf, strlen(send_buf) * sizeof(char), 0) == -1)
                {
                    LOG_ERROR("Error on send() call");
                }
            }
            else
            {
                LOG_ERROR("Error on recv() call");
            }
        } while (data_recv > 0);

        close(s2);
    }

    return 0;
}
