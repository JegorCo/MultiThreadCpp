#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> 

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr, client_addr; 
    socklen_t client_addr_len = sizeof(client_addr); 
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Invalid sock\n";
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid adress\n";
        return -1;
    }

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Invalid sock\n";
        return -1;
    }
    
    if (listen(sock, 3) < 0) {
        std::cout << "Connecion error\n";
        return -1;
    }

    int new_socket;
    while (true) {
        std::cout << "Waiting connection...\n";

        if ((new_socket = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            std::cout << "\n";
            return -1;
        }

        valread = read(new_socket, buffer, sizeof(buffer) - 1);
        if (valread <= 0) {
            std::cout << "Data reading error, try to input sting\n";
            close(new_socket);
            continue;
        }

        // Завершите строку нулевым символом
        buffer[valread] = '\0';

        // Преобразуем строку в целое число
        int message = std::stoi(buffer);

        if ((message / 100 != 0) && (message % 32 == 0)) {
            std::cout << "Data incorrect\n";
        } else {
            std::cout << "Length of message: " << std::to_string(message).size() << "\n";
        }

        // Очищаем буфер
        memset(buffer, 0, sizeof(buffer));

        close(new_socket);
    }

    close(sock);
    return 0;
}