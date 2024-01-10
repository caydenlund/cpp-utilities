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

module;

#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

export module testing;

namespace testing {

    /**
     * @brief Runs all of the given tests.
     *
     * @param tests The tests to run.
     * @param use_colors Whether to use colors or not. Defaults to true.
     */
    export void run_all(const std::vector<std::tuple<std::function<void()>, std::string>>& tests,
                        bool use_colors = true) {
        if (tests.empty()) {
            std::cout << "No tests to run.\n";
            return;
        }

        const std::string passed_color = (use_colors) ? "\033[32m" : "";
        const std::string failed_color = (use_colors) ? "\033[31m" : "";
        const std::string reset_color = (use_colors) ? "\033[0m" : "";

        constexpr unsigned int base = 10;
        const std::function<unsigned int(unsigned int)> log_10 = [&log_10](unsigned int number) {
            if (number < base) return (unsigned int)1;
            return log_10(number / base) + 1;
        };

        const size_t num_tests = tests.size();
        const unsigned int num_tests_digits = log_10(num_tests);
        std::cout << "Running " << num_tests << " test" << ((num_tests == 1) ? "" : "s") << ".\n";

        unsigned int longest_test_string = 0;
        for (const auto& test : tests) {
            longest_test_string = std::max(longest_test_string, (unsigned int)std::get<1>(test).size());
        }

        size_t num_passed = 0;

        for (size_t index = 0; index < num_tests; ++index) {
            const auto& test = tests[index];
            const std::string& test_name = std::get<1>(test);
            const auto test_number = (unsigned int)index + 1;
            const auto num_padding = num_tests_digits - log_10(test_number);
            std::cout << "    [" << std::string(num_padding, ' ') << index + 1 << "/" << num_tests << "]:  \""
                      << test_name << "\":  " << std::string(longest_test_string - test_name.size(), ' ');

            try {
                std::get<0>(test)();
                std::cout << passed_color << "[PASSED]" << reset_color << '\n';
                ++num_passed;
            } catch (const std::exception& err) {
                std::cout << failed_color << "[FAILED]" << reset_color << '\n'
                          << "        Error message: \"" << err.what() << "\"\n";
            }
        }

        std::cout << "Passed " << num_passed << "/" << num_tests << " test" << ((num_tests == 1) ? "" : "s") << ".\n";
    }

    /*
    ===================
    ||  Assertions:  ||
    ===================
    */

    /**
     * @brief Asserts that the two given items are equal.
     * @details Throws an exception on failure.
     *
     * @tparam obj_t The type of the items.
     * @param lhs The first item to compare.
     * @param rhs The second item to compare.
     * @param message The error message on failure.
     */
    export template<typename obj_t>
    void assert_eq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs == rhs)) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the two given items are not equal.
     * @details Throws an exception on failure.
     *
     * @tparam obj_t The type of the items.
     * @param lhs The first item to compare.
     * @param rhs The second item to compare.
     * @param message The error message on failure.
     */
    export template<typename obj_t>
    void assert_neq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs != rhs)) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the first given item is less than or equal to the second item.
     * @details Throws an exception on failure.
     *
     * @tparam obj_t The type of the items.
     * @param lhs The first item to compare.
     * @param rhs The second item to compare.
     * @param message The error message on failure.
     */
    export template<typename obj_t>
    void assert_leq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs <= rhs)) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the first given item is greater than or equal to the second item.
     * @details Throws an exception on failure.
     *
     * @tparam obj_t The type of the items.
     * @param lhs The first item to compare.
     * @param rhs The second item to compare.
     * @param message The error message on failure.
     */
    export template<typename obj_t>
    void assert_geq(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs >= rhs)) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the first given item is less than the second item.
     * @details Throws an exception on failure.
     *
     * @tparam obj_t The type of the items.
     * @param lhs The first item to compare.
     * @param rhs The second item to compare.
     * @param message The error message on failure.
     */
    export template<typename obj_t>
    void assert_lt(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs < rhs)) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the first given item is greater than the second item.
     * @details Throws an exception on failure.
     *
     * @tparam obj_t The type of the items.
     * @param lhs The first item to compare.
     * @param rhs The second item to compare.
     * @param message The error message on failure.
     */
    export template<typename obj_t>
    void assert_gt(const obj_t& lhs, const obj_t& rhs, const std::string& message) {
        if (!(lhs > rhs)) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the given condition is true.
     * @details Throws an exception on failure.
     *
     * @param cond The condition to test.
     * @param message The error message on failure.
     */
    export void assert_true(bool cond, const std::string& message) {
        if (!cond) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the given condition is false.
     * @details Throws an exception on failure.
     *
     * @param cond The condition to test.
     * @param message The error message on failure.
     */
    export void assert_false(bool cond, const std::string& message) {
        if (cond) throw std::runtime_error(message);
    }

    /**
     * @brief Asserts that the given function throws the given type of exception.
     * @details Throws an exception on failure.
     *
     * @tparam err_t The type of exception that should be thrown. Defaults to `std::exception`.
     * @param func The function that should through the given type of exception.
     * @param message The error message on failure.
     */
    export template<typename err_t = std::exception>
    void assert_throws(const std::function<void()>& func, const std::string& message) {
        try {
            func();
        } catch (const err_t& err) { return; } catch (const std::exception& err) {
            throw std::runtime_error(message);
        }
    }

    /**
     * @brief Asserts that the given function throws the given type of exception.
     * @details Checks whether the thrown exception has the correct (`::what()`) message.
     *     Throws an exception on failure.
     *
     * @tparam err_t The type of exception that should be thrown. Defaults to `std::exception`.
     * @param func The function that should through the given type of exception.
     * @param expected_message The message that the thrown exception should report.
     * @param message The error message on failure.
     */
    export template<typename err_t = std::exception>
    void assert_throws(const std::function<void()>& func, const std::string& expected_message,
                       const std::string& message) {
        try {
            func();
        } catch (const err_t& err) {
            assert_eq(std::string(err.what()), expected_message, message);
        } catch (const std::exception& err) { throw std::runtime_error(message); }
    }

}  //  namespace testing
