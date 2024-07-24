#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class implementation {
private:
    std::mutex mtx;
    char buffer[1024] = {0};

    int ValidStr(const char* str) {
        size_t len = strlen(str);
        if (len > 64) {
            return false;
        }
        for (size_t i = 0; i < len; i++) {
            if (!std::isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    std::vector<int> StrToSorted(const char* str) {
        std::vector<int> string_array;
        for (size_t i = 0; i < strlen(str); i++) {
            string_array.push_back(str[i] - '0'); 
        }
        std::sort(string_array.begin(), string_array.end(), std::greater<int>());
        return string_array;
    }

    void replaceAll(std::string &str, const std::string &from, const std::string &to) {
        size_t startPos = 0;
        while ((startPos = str.find(from, startPos)) != std::string::npos) {
            str.replace(startPos, from.length(), to);
            startPos += to.length();
        }
    }

    std::string arrayToString(const std::vector<int>& array) {
        std::ostringstream oss;
        for (size_t i = 0; i < array.size(); i++) {
            oss << array[i];
        }
        return oss.str();
    }

    int StrSum(const char* str) {
        int sum = 0;
        for (size_t i = 0; i < strlen(str); i++) {
            if (std::isdigit(str[i])) {
                sum += int(str[i]) - int('0');
            }
        }
        return sum;
    }

    void sendToServer(const std::string &data) {
        int sock = 0;
        struct sockaddr_in ip_addr;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            std::cout << "Invalid socket\n";
            return;
        }

        ip_addr.sin_family = AF_INET;
        ip_addr.sin_port = htons(8080);

        if (inet_pton(AF_INET, "127.0.0.1", &ip_addr.sin_addr) <= 0) {
            std::cout << "Invalid IP\n";
            return;
        }

        if (connect(sock, (struct sockaddr *)&ip_addr, sizeof(ip_addr)) < 0) {
            std::cout << "Connection error, try to run program2\n";
            return;
        }

        send(sock, data.c_str(), data.length(), 0);
        close(sock);
    }

public:
    void inputFunction() {
        mtx.lock();
        char user_input[65] = {0}; 
        std::cin >> user_input;
        if (ValidStr(user_input)) {
            std::vector<int> arr = StrToSorted(user_input);
            std::string newst = arrayToString(arr);
            const char* fromList[] = {"2", "4", "6", "8", "0"};

            for (const auto &from : fromList) {
                replaceAll(newst, from, "KB"); 
            }

            strncpy(buffer, newst.c_str(), sizeof(buffer) - 1); 
            buffer[sizeof(buffer) - 1] = '\0'; 

        } else {
            std::cout << "Invalid input\n";
        }
        mtx.unlock();
    }

    void processBuffer() {
        mtx.lock(); 
        std::cout << buffer << "\n";
        int topr2 = StrSum(buffer);
        sendToServer(std::to_string(topr2));
        std::cout << topr2 << "\n";
        buffer[0] = {0}; 
        mtx.unlock();
    }
};

int main() {
    implementation impl;

    while (true) {
        std::thread input_thread(&implementation::inputFunction, &impl);
        std::thread processing_thread(&implementation::processBuffer, &impl);

        input_thread.join();
        processing_thread.join();
    }
    return 0;
}