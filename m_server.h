#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "m_globals.h"

static sockaddr_in client_addr;

bool createServer(){
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Помилка створення сокета");
        return false;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // --- ДОДАЄМО ТАЙМ-АУТ ДЛЯ ACCEPT ---
    struct timeval timeout;
    timeout.tv_sec = 1;  // 1 секунда
    timeout.tv_usec = 0;
    // SO_RCVTIMEO дозволяє accept() виходити за тайм-аутом
    setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));


    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(C_TcpPort);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cout<<"Помилка bind"<<std::endl;
        close(server_fd);
        return false;
    }

    listen(server_fd, 5);

    return true;
}