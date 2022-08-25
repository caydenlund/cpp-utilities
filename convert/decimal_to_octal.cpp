/**
 * @file decimal_to_octal.cpp
 * @brief Converts a decimal number to octal notation.
 * @details Decimal number may be given as a program argument or from stdin.
 *
 * @author Cayden Lund
 * @date 25 August 2022
 *
 * @copyright MIT License <https://opensource.org/licenses/MIT>.
 */

#include <iostream>
#include <string>

#include "convert.hpp"

/**
 * @brief Main program entry point.
 *
 * @param argc The number of program arguments given.
 * @param argv The argument array.
 * @return 0 on success, 1 on failure.
 */
int main(int argc, char** argv) {
    std::string input;
    if (argc < 2) {
        std::getline(std::cin, input);
    } else {
        input = std::string(argv[1]);
    }

    convert c(input, convert::types::decimal);
    std::cout << c.as_octal() << std::endl;

    return 0;
}