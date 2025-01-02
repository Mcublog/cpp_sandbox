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
#define LOG_MODULE_NAME clnt
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

static const char *kSocketPath = "mysocket";
static constexpr unsigned int kRecvLen = 200;
static constexpr unsigned int kSendLen = 100;

int main()
{
    int sock = 0;
    int data_len = 0;
    struct sockaddr_un remote;
    char recv_msg[kRecvLen];
    char send_msg[kSendLen];

    memset(recv_msg, 0, kRecvLen * sizeof(char));
    memset(send_msg, 0, kSendLen * sizeof(char));

    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        LOG_ERROR("Client: Error on socket() call");
        return 1;
    }

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, kSocketPath);

    LOG_INFO("Client: Trying to connect...");
    if (connect(sock, (struct sockaddr *)&remote, sizeof(remote)) == -1)
    {
        LOG_ERROR("Client: Error on connect call");
        return 1;
    }

    LOG_INFO("Client: Connected");

    while (printf(">"), fgets(send_msg, kSendLen, stdin), !feof(stdin))
    {
        if (send(sock, send_msg, strlen(send_msg) * sizeof(char), 0) == -1)
        {
            LOG_ERROR("Client: Error on send() call");
        }
        memset(send_msg, 0, kSendLen * sizeof(char));
        memset(recv_msg, 0, kRecvLen * sizeof(char));

        if ((data_len = recv(sock, recv_msg, kRecvLen, 0)) > 0)
        {
            LOG_INFO("Client: Data received: %s", recv_msg);
        }
        else
        {
            if (data_len < 0)
            {
                LOG_ERROR("Client: Error on recv() call");
            }
            else
            {
                LOG_INFO("Client: Server socket closed");
                close(sock);
                break;
            }
        }
    }

    LOG_INFO("Client: bye!");

    return 0;
}
