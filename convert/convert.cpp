/**
 * @brief Implementation of the `convert` class.
 * @details See header file for documentation.
 *
 * @author Cayden Lund
 * @date 25 August 2022
 *
 * @copyright MIT License <https://opensource.org/licenses/MIT>.
 */

#include "convert.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>

void convert::save_binary(const std::string &binary_number) {
    // First, if the string is empty or zero, just save the empty string.
    if (binary_number.empty() ||
        binary_number == std::string(binary_number.size(), '0')) {
        this->_binary_num = "";
        return;
    }

    // Trim leading zeroes.
    std::string trimmed_number = binary_number;
    for (size_t index = 0; index < trimmed_number.size(); index++) {
        if (trimmed_number[index] == '1') {
            trimmed_number = trimmed_number.substr(index);
            break;
        }
    }

    // Make sure that all characters are legal:
    for (const char &c : trimmed_number) {
        if (c != '0' && c != '1') {
            std::cerr << "Invalid character: " << c << std::endl;
            exit(1);
        }
    }

    this->_binary_num = trimmed_number;
}

void convert::save_decimal(const std::string &decimal_number) {
    unsigned long long total = 0;

    std::string binary_number(decimal_number.length(), ' ');

    // Ensure that the string will fit inside an `unsigned long long`.
    if (decimal_number.size() > 20) {
        std::cerr << "Overflow error; number is too large." << std::endl;
        exit(1);
    } else if (decimal_number.size() == 20) {
        std::string maximum = "18446744073709551615";

        for (size_t index = 0; index < decimal_number.size(); index++) {
            if (decimal_number[index] > maximum[index]) {
                std::cerr << "Overflow error; number is too large."
                          << std::endl;
                exit(1);
            } else if (decimal_number[index] < maximum[index]) {
                break;
            }
        }
    }

    // Process each digit:
    for (size_t index = 0; index < decimal_number.length(); index++) {
        char c = decimal_number[index];
        unsigned long long value = c - '0';

        if (value > 9) {
            std::cerr << "Invalid character: " << c << std::endl;
            exit(1);
        }

        // Using left-shifts, quickly scale the digit to the correct magnitude.
        for (size_t power_of_ten = 0;
             power_of_ten < decimal_number.length() - index - 1;
             power_of_ten++) {
            unsigned long long double_value = (value << 1);
            value <<= 3;
            value += double_value;
        }

        total += value;
    }

    std::stringstream binary_string;

    unsigned long long bit = 1;
    unsigned short binary_counter = 0;

    while (bit <= total && bit > 0) {
        bit <<= 1;
        binary_counter++;
    }

    // Reset the bit from the binary counter,
    // in case an overflow occurred.
    bit = 1;
    bit <<= binary_counter - 1;

    // Test each bit until we've found the whole `total` in binary form.
    while (total > 0 && bit > 0) {
        if (total >= bit) {
            total -= bit;
            binary_string << "1";
        } else {
            binary_string << "0";
        }

        bit >>= 1;
    }

    // End with zeroes, if necessary:
    while (bit > 0) {
        bit >>= 1;
        binary_string << "0";
    }

    this->save_binary(binary_string.str());
}

void convert::save_hex(const std::string &hex_number) {
    std::stringstream result;

    for (const char &c : hex_number) {
        int value;
        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else {
            switch (c) {
                case 'a':
                case 'A':
                    value = 10;
                    break;
                case 'b':
                case 'B':
                    value = 11;
                    break;
                case 'c':
                case 'C':
                    value = 12;
                    break;
                case 'd':
                case 'D':
                    value = 13;
                    break;
                case 'e':
                case 'E':
                    value = 14;
                    break;
                case 'f':
                case 'F':
                    value = 15;
                    break;
                default:
                    std::cerr << "Invalid character: " << c << std::endl;
                    exit(1);
            }
        }

        // For each bit in the `value` number, set the corresponding "bit"
        // in the string.
        for (unsigned short index = 0; index < 4; index++) {
            unsigned short bit = (value >> (3 - index)) & 1;

            // Skip leading zeroes.
            if (result.str().empty() && bit == 0) continue;

            result << bit;
        }
    }

    this->save_binary(result.str());
}

void convert::save_octal(const std::string &octal_number) {
    std::stringstream result;

    for (const char &c : octal_number) {
        int value = c - '0';
        if (value < 0 || value > 7) {
            std::cerr << "Invalid character: " << c << std::endl;
            exit(1);
        }

        // For each bit in the `value` number, set the corresponding "bit"
        // in the string.
        for (unsigned short index = 0; index < 3; index++) {
            unsigned short bit = (value >> (2 - index)) & 1;

            // Skip leading zeroes.
            if (result.str().empty() && bit == 0) continue;

            result << bit;
        }
    }

    this->save_binary(result.str());
}

convert::convert(std::string num, convert::types type) {
    // Remove spaces, if present:
    num.erase(std::remove(num.begin(), num.end(), ' '), num.end());

    switch (type) {
        case types::binary:
            this->save_binary(num);
            break;
        case types::decimal:
            this->save_decimal(num);
            break;
        case types::hex:
            // If the string starts with "0x", remove it.
            if (num.size() >= 2 && num.substr(0, 2) == "0x") {
                num = num.substr(2);
            }

            this->save_hex(num);
            break;
        case types::octal:
            this->save_octal(num);
            break;
    }
}

std::string convert::as_binary() {
    std::stringstream output;
    std::string binary_num = this->_binary_num;

    // Insert a space between every byte, for readability.
    for (size_t index = binary_num.size() - 8; index > 0; index -= 8) {
        // Since `size_t` may be an unsigned data type, make an extra check:
        if (index >= this->_binary_num.length()) break;

        binary_num.insert(index, " ");
    }

    output << "Raw binary:      ";

    if (binary_num.empty()) {
        output << "0";
    }

    // Insert leading zeroes to pad to a full byte, if necessary.
    output << std::string((8 - this->_binary_num.size() % 8) % 8, '0');

    output << binary_num;

    // If it has 32 or 64 digits, also output it formatted in the floating-point
    // format.
    // I.e.,  [s] [e] [f]
    if (this->_binary_num.length() <= 32) {
        output << std::endl << "As IEEE float:   ";

        std::string float_bin =
            std::string(32 - this->_binary_num.length(), '0') +
            this->_binary_num;

        // [s]
        output << float_bin[0] << " ";

        // [e]
        output << float_bin.substr(1, 8) << " ";

        // [f]
        output << float_bin.substr(9) << " ";
    }

    if (this->_binary_num.length() <= 64) {
        output << std::endl << "As IEEE double:  ";

        std::string double_bin =
            std::string(64 - this->_binary_num.length(), '0') +
            this->_binary_num;

        // [s]
        output << double_bin[0] << " ";

        // [e]
        output << double_bin.substr(1, 11) << " ";

        // [f]
        output << double_bin.substr(12) << " ";
    }

    return output.str();
}

std::string convert::as_decimal() {
    std::stringstream output;
    std::string binary_num = this->_binary_num;

    if (this->_binary_num.empty()) {
        binary_num = "0";
    }

    if (binary_num.size() <= 64) {
        unsigned long long total = 0;

        for (size_t index = 0; index < binary_num.size(); index++) {
            size_t shift = binary_num.size() - index - 1;

            if (binary_num[index] == '0') continue;

            total += (1UL << shift);
        }

        output << "As unsigned integer: " << total;

        if ((binary_num.size() == 32 || binary_num.size() == 64) &&
            binary_num[0] == '1') {
            unsigned long long to_subtract = 1;
            to_subtract <<= binary_num.size() - 1;
            output << std::endl
                   << "As signed integer:   -" << total - to_subtract;
        }

        if (binary_num.size() <= 32) {
            std::string float_bin =
                std::string(32 - this->_binary_num.length(), '0') +
                this->_binary_num;

            // output << std::endl << "As IEEE float:       " << float_bin;
            // TODO: As float.
        }
        if (binary_num.size() <= 64) {
            std::string double_bin =
                std::string(64 - this->_binary_num.length(), '0') +
                this->_binary_num;

            // output << std::endl << "As IEEE double:      " << double_bin;
            // TODO: As double.
        }
    }

    return output.str();
}

std::string convert::as_hex() {
    if (this->_binary_num.empty()) {
        return "0x0";
    }

    std::stringstream output;
    output << "0x";

    for (size_t index = this->_binary_num.size() % 4;
         index <= this->_binary_num.size(); index += 4) {
        std::string word;

        if (index == 0) {
            continue;
        } else if (index < 4) {
            word = std::string(4 - index, '0') +
                   this->_binary_num.substr(0, index);
        } else {
            word = this->_binary_num.substr(index - 4, 4);
        }

        int total = 0;
        for (unsigned short word_index = 0; word_index < 4; word_index++) {
            unsigned short value = word[3 - word_index] - '0';
            total += value << word_index;
        }

        // If it's a single-digit number, then we can just add it to the output.
        if (total < 10) {
            output << total;
        } else {
            // Otherwise, we need to translate it to a letter.
            output << std::string(1, (char)(total + 'A' - 10));
        }
    }

    return output.str();
}

std::string convert::as_octal() {
    if (this->_binary_num.empty()) {
        return "0";
    }

    std::stringstream output;

    for (size_t index = this->_binary_num.size() % 3;
         index <= this->_binary_num.size(); index += 3) {
        std::string word;

        if (index == 0) {
            continue;
        } else if (index < 3) {
            word = std::string(3 - index, '0') +
                   this->_binary_num.substr(0, index);
        } else {
            word = this->_binary_num.substr(index - 3, 3);
        }

        int total = 0;
        for (unsigned short word_index = 0; word_index < 3; word_index++) {
            unsigned short value = word[2 - word_index] - '0';
            total += value << word_index;
        }

        // If it's a single-digit number, then we can just add it to the output.
        output << total;
    }

    return output.str();
}
