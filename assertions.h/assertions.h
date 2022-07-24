/**
 * @file assertions.h
 * @author Cayden Lund (cayden.lund@utah.edu)
 * @brief Header-only library for making assertions.
 * @details Useful for unit testing and debugging.
 * @date 19 July 2022
 *
 * @copyright Copyright (c) 2022 by Cayden Lund.
 */

#ifndef ASSERT_H
#define ASSERT_H

#include <iostream>
#include <string>

namespace assert {
/**
 * @brief Asserts that a condition is true.
 *
 * @param condition The condition to assert.
 * @param name The name of the condition.
 */
void is_true(bool condition, std::string message = "");

/**
 * @brief Asserts that a condition is false.
 *
 * @param condition The condition to assert.
 * @param name The name of the condition.
 */
void is_false(bool condition, std::string message = "");

/**
 * @brief Asserts that two values are equal.
 *
 * @param a The first value.
 * @param b The second value.
 * @param name The name of the values.
 */
template <typename T>
void are_equal(T a, T b, std::string message = "");

/**
 * @brief Asserts that two values are not equal.
 *
 * @param a The first value.
 * @param b The second value.
 * @param name The name of the values.
 */
template <typename T>
void are_not_equal(T a, T b, std::string message = "");
}  // namespace assert

void assert::is_true(bool condition, std::string message) {
    if (!condition) {
        if (message.empty()) {
            std::cout << "Assertion failed: the condition is not true."
                      << std::endl;
        } else {
            std::cout << "Assertion failed: the condition \"" << message
                      << "\" is not true." << std::endl;
        }
        exit(1);
    }
}

void assert::is_false(bool condition, std::string message) {
    if (condition) {
        if (message.empty()) {
            std::cout << "Assertion failed: the condition is not false."
                      << std::endl;
        } else {
            std::cout << "Assertion failed: the condition \"" << message
                      << "\" is not false." << std::endl;
        }
        exit(1);
    }
}

template <typename T>
void assert::are_equal(T a, T b, std::string message) {
    if (a != b) {
        if (message.empty()) {
            std::cout << "Assertion failed: the values are not equal."
                      << std::endl
                      << "    Value 1: " << a << std::endl
                      << "    Value 2: " << b << std::endl;
        } else {
            std::cout << "Assertion failed: the values \"" << message
                      << "\" are not equal." << std::endl
                      << "    Value 1: " << a << std::endl
                      << "    Value 2: " << b << std::endl;
        }
        exit(1);
    }
}

template <typename T>
void assert::are_not_equal(T a, T b, std::string message) {
    if (a == b) {
        if (message.empty()) {
            std::cout << "Assertion failed: the values are equal." << std::endl
                      << "    Value 1: " << a << std::endl
                      << "    Value 2: " << b << std::endl;
        } else {
            std::cout << "Assertion failed: the values \"" << message
                      << "\" are equal." << std::endl
                      << "    Value 1: " << a << std::endl
                      << "    Value 2: " << b << std::endl;
        }
        exit(1);
    }
}

#endif
