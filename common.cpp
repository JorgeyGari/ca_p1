/* Source file containing all functions common to all elements */

#include "common.hpp"
#include <iostream>
#include <chrono>

void err_msg(ErrorType error)
// Receives a member of the ErrorType class and displays its corresponding error message
{
    switch (error) {
        case ErrorType::unopened_file:
            std::cout << "The specified file does not exist or could not be opened.\n";
            break;
        case ErrorType::wrong_type:
            std::cout << "The specified file is not a bitmap image.\n";
            break;
        case ErrorType::wrong_planes:
            std::cout << "The specified bitmap is invalid (number of plains is not equal to 1).\n";
            break;
        case ErrorType::wrong_point_size:
            std::cout << "The specified bitmap is invalid (point size is not equal to 24).\n";
            break;
        case ErrorType::wrong_compression:
            std::cout << "The specified bitmap is invalid (compression value is not equal to 0).\n";
            break;
    }
    exit(-1);// Finish the execution after the error message is displayed with error code -1
}

void print_data(const std::string &op, long loadtime, long opertime, long storetime) {
    std::cout << "Load time: " << loadtime << "\n";
    std::cout << op << " time: " << opertime << "\n";
    std::cout << "Store time: " << storetime << "\n----------------------\n";
}

long stop_chrono(std::chrono::time_point<std::chrono::system_clock> start) {
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    auto time = duration.count();
    return time;
}
