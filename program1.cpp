#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <time.h>
class imlementation {
private:
    std::mutex mtx;
    std::string buffer;

    int ValidStr(std::string str) {
        if (str.size() > 64) {
            return false;
        } else {
            for (int i = 0; i < str.size(); i++) {
                if (std::isdigit(str[i])) {
                    continue;
                } else {
                    return false;
                    break;
                }
            }
            return true;
        }
    }

    std::vector<int> StrToSorted(std::string str) {
        std::vector<int> string_array;
        for (int i = 0; i < str.size(); i++) {
            string_array.push_back(str[i] - '0');
        }

        std::sort(string_array.begin(), string_array.end(), std::greater<int>());
        return string_array;
    }

    void replaceAll(std::string &str, const std::string &from, const std::string &to) {
        size_t startPos = 0;
        while (true) {
            startPos = str.find(from, startPos);
            if (startPos == std::string::npos) {
                break;
            }
            str.replace(startPos, from.length(), to);
            startPos += to.length();
        }
    }


    std::string arrayToString(const std::vector<int> &array) {
        std::ostringstream oss;
        for (int i = 0; i < array.size(); i++) {
            if (i != 0) {
                oss << "";
            }
            oss << array[i];
        }
        return oss.str();
    }


    int StrSum(std::string str) {
        int sum = 0;
        for (int i = 0; i < str.size(); i++) {
            if (std::isdigit(str[i])) {
                sum += int(str[i]) - int('0');
                continue;
            }
        }
        return sum;
    }


public:
    void inputFunction() {

        mtx.lock();
        std::string user_input;
        std::cin >> user_input;
        bool verify;
        verify = ValidStr(user_input);
        if (verify) {

            std::vector<int> arr = StrToSorted(user_input);

            std::string newst;

            newst = arrayToString(arr);

            std::vector<std::string> fromList = {"2", "4", "6", "8", "0"};

            for (const auto &from: fromList) {
                replaceAll(newst, from, "KB");
            }

            //std::cout << newst << "\n";
            buffer = newst;

        } else {
            std::cout << "Invalid input\n";
        }
        mtx.unlock();

    }

    void processBuffer() {

        std::cout << buffer << "\n";
        int topr2 = StrSum(buffer);
        buffer.clear();


    }
};

int main() {

    imlementation impl;

    while(true) {
        std::thread input_thread(&imlementation::inputFunction, &impl);
        std::thread processing_thread(&imlementation::processBuffer, &impl);

        input_thread.join();
        processing_thread.join();
    }
    return 0;
}