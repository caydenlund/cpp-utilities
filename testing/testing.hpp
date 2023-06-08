/**
 * @file testing.hpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief Defines several top-level functions for unit testing.
 * @details A function is defined (`testing::run_all(const std::vector<test_t>& tests)`) for running all given tests.
 *     A test (`test_t`) is a pair of a function with no arguments and a void return value, and a string name
 *     (`std::tuple<std::function<void()>, std::string>`).
 *
 *     A test fails when an exception is thrown; it passes otherwise. This function prints output to the console.
 *
 *     Several assertions are defined:
 *
 *         * `assert_eq<obj_t>(obj_t a, obj_t b, std::string message)`:
 *           Asserts that object `a` is equal (`==`) to object `b`.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_neq<obj_t>(obj_t a, obj_t b, std::string message)`:
 *           Asserts that object `a` is not equal (`!=`) to object `b`.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_leq<obj_t>(obj_t a, obj_t b, std::string message)`:
 *           Asserts that object `a` is less than or equal (`<=`) to object `b`.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_geq<obj_t>(obj_t a, obj_t b, std::string message)`:
 *           Asserts that object `a` is greater than or equal (`>=`) to object `b`.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_lt<obj_t>(obj_t a, obj_t b, std::string message)`:
 *           Asserts that object `a` is less (`<`) than object `b`.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_gt<obj_t>(obj_t a, obj_t b, std::string message)`:
 *           Asserts that object `a` is greater (`>`) than object `b`.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_true(bool cond, std::string message)`:
 *           Asserts that the given condition is true.
 *           Throws an exception with the given message otherwise.
 *
 *         * `assert_false(bool cond, std::string message)`:
 *           Asserts that the given condition is false.
 *           Throws an exception with the given message otherwise.
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#ifndef TESTING_HPP
#define TESTING_HPP

#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace testing {

    /**
     * @brief Runs all of the given tests.
     *
     * @param tests The tests to run.
     * @param use_colors Whether to use colors or not. Defaults to true.
     */
    void run_all(const std::vector<std::tuple<std::function<void()>, std::string>>& tests, bool use_colors = true) {
        if (tests.empty()) {
            std::cout << "No tests to run." << std::endl;
            return;
        }

        const std::string passed_color = (use_colors) ? "\033[32m" : "";
        const std::string failed_color = (use_colors) ? "\033[31m" : "";
        const std::string reset_color = (use_colors) ? "\033[0m" : "";

        const size_t num_tests = tests.size();
        std::cout << "Running " << num_tests << " test" << ((num_tests > 1) ? "s" : "") << "." << std::endl;

        size_t num_passed = 0;

        for (size_t index = 0; index < num_tests; ++index) {
            const auto& test = tests[index];
            std::cout << "    [" << index + 1 << "/" << num_tests << "]: \"" << std::get<1>(test) << "\": ";

            try {
                std::get<0>(test)();
                std::cout << passed_color << "[PASSED]" << reset_color << std::endl;
                ++num_passed;
            } catch (const std::exception& err) {
                std::cout << failed_color << "[FAILED]" << reset_color << std::endl
                          << "        Error message: \"" << err.what() << "\"" << std::endl;
            }
        }

        std::cout << "Passed " << num_passed << "/" << num_tests << " test" << ((num_tests > 1) ? "s" : "") << "."
                  << std::endl;
    }

    /*
    ===================
    ||  Assertions:  ||
    ===================
    */

    template<typename obj_t>
    void assert_eq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs == rhs)) throw std::runtime_error(message);
    }

    template<typename obj_t>
    void assert_neq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs != rhs)) throw std::runtime_error(message);
    }

    template<typename obj_t>
    void assert_leq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs <= rhs)) throw std::runtime_error(message);
    }

    template<typename obj_t>
    void assert_geq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs >= rhs)) throw std::runtime_error(message);
    }

    template<typename obj_t>
    void assert_lt(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs < rhs)) throw std::runtime_error(message);
    }

    template<typename obj_t>
    void assert_gt(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs > rhs)) throw std::runtime_error(message);
    }

    void assert_true(bool cond, const std::string& message) {
        if (!cond) throw std::runtime_error(message);
    }

    void assert_false(bool cond, const std::string& message) {
        if (cond) throw std::runtime_error(message);
    }

}  //  namespace testing

#endif  //  TESTING_HPP
