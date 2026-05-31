#include <iostream>
#include <cstring>
#include <csignal> // Для роботи з сигналами
#include <algorithm>
#include <string>

#include "m_globals.h"
#include "m_server.h"
#include "m_processData.h"
#include "m_functions.h"

// Глобальна змінна для контролю циклу
bool keep_running = true;


// Обробник сигналу (Ctrl+C)
void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\n[!] Отримано сигнал завершення (Ctrl+C). Закриваємо сервер..." << std::endl;
        keep_running = false;
    }
}

int main() {
    // Реєструємо обробник сигналу SIGINT
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGKILL, signal_handler);

    if(createServer()){
        std::cout << "Сервер працює на порту "<<C_TcpPort<<". Натисніть Ctrl+C для виходу." << std::endl;
    } else {
        std::cout << "Не можу створити сервер на порту "<<C_TcpPort<<". Виходжу." << std::endl;
        return 1;
    }


    // Головний цикл тепер залежить від keep_running
    char buffer[1024] = {0};
    std::string buffer_str;
    buffer_str.resize(1024);

    while (keep_running) {
        socklen_t addr_len = sizeof(client_addr);

        // ВАЖЛИВО: accept() — це блокуюча операція.
        // На Linux Ctrl+C перерве accept() і вона поверне -1.
        int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

        if (client_socket < 0) {	
            continue;
        }

        std::cout << "Клієнт підключився: " << inet_ntoa(client_addr.sin_addr) << std::endl;

        buffer_str = "";
        buffer_str.resize(1024);
        int data_size = read(client_socket, buffer_str.data(), buffer_str.size());
        buffer_str = buffer_str.substr(0, data_size);

        if(processCommand(client_socket, buffer_str)){
            continue;
        } else {
            std::cout<<"Помилка обробки команди: "<<buffer_str<<std::endl;
        }

        send(client_socket, C_unknown.data(), C_unknown.size(), 0);

        close(client_socket);
    }
    start_stream("", "", false);

    // Коректне закриття основного сокета перед виходом
    std::cout << "Очищення ресурсів..." << std::endl;
    close(server_fd);
    std::cout << "Сервер зупинено." << std::endl;

    return 0;
}
