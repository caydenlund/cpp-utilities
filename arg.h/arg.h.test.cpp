/**
 * @file arg.h.test.cpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 * @brief Tests the arg.h library.
 * @date 27 August 2022
 *
 * @copyright Copyright (c) 2022 Cayden Lund
 *
 */

#include "arg.h"

#include <cstring>
#include <string>

#include "assertions.h/assertions.h"

/**
 * @brief Used to convert a list of strings into the char** format.
 * @details Auto-manages its own memory.
 *
 */
struct char_arr_arr {
    /**
     * @brief The count of arguments.
     *
     */
    int argc;

    /**
     * @brief The array of arguments.
     *
     */
    char **argv;

    /**
     * @brief Implicit constructor of string list to char** array.
     *
     * @param str_list The array of arguments in a string list format.
     */
    [[maybe_unused]] char_arr_arr(
        const std::initializer_list<std::string> &str_list) {
        this->argc = (int)str_list.size();
        this->argv = new char *[argc];

        size_t i = 0;
        for (const std::string &arg : str_list) {
            this->argv[i] = new char[arg.size() + 1];
            strcpy(this->argv[i], arg.c_str());
            i++;
        }
    }

    /**
     * @brief Copy constructor.
     *
     * @param other The other `char_arr_arr` object to copy.
     */
    char_arr_arr(const char_arr_arr &other) {
        this->argc = other.argc;
        this->argv = new char *[argc];

        for (size_t i = 0; i < argc; i++) {
            strcpy(this->argv[i], other.argv[i]);
        }
    }

    /**
     * @brief Assignment operator.
     *
     * @param other The other `char_arr_arr` to assign to this.
     */
    char_arr_arr &operator=(const char_arr_arr &other) {
        if (this == &other) {
            return *this;
        }

        char_arr_arr temporary(other);
        std::swap(this->argc, temporary.argc);
        std::swap(this->argv, temporary.argv);

        return *this;
    }

    /**
     * @brief Destructor.
     *
     */
    ~char_arr_arr() {
        for (size_t i = 0; i < argc; i++) {
            delete[] this->argv[i];
        }

        delete[] this->argv;
    }
};

/**
 * @brief Simple test.
 * @details Ensures that the `argh` instance can be constructed without throwing
 *     an exception, and that it can correctly save all arguments passed to it.
 *
 *     Does not use any "option" (leading dash) arguments.
 *
 */
void simple_test() {
    std::cout << "Running simple test." << std::endl << std::endl;

    char_arr_arr test_args = {"program", "1", "2", "3"};
    argh test_argh(test_args.argc, test_args.argv);

    assert::are_equal(test_argh.size(), (size_t)test_args.argc,
                      "Simple test: size().");

    assert::are_equal(test_argh[0], std::string("program"),
                      "Simple test: positional test [0]");
    assert::are_equal(test_argh[1], std::string("1"),
                      "Simple test: positional test [1]");
    assert::are_equal(test_argh[2], std::string("2"),
                      "Simple test: positional test [2]");
    assert::are_equal(test_argh[3], std::string("3"),
                      "Simple test: positional test [3]");
    assert::are_equal(test_argh[4], std::string(),
                      "Simple test: positional test [4]");
}

/**
 * @brief Option test.
 * @details Ensures that the `argh` instance can be correctly differentiate
 *     between option and non-option arguments.
 *
 */
void option_test() {
    std::cout << "Running option test." << std::endl << std::endl;

    char_arr_arr test_args = {"program", "-1", "2", "-3"};
    argh test_argh(test_args.argc, test_args.argv);

    assert::are_equal(test_argh.size(), (size_t)2, "Option test: size().");

    assert::are_equal(test_argh[0], std::string("program"),
                      "Option test: positional test [0]");
    assert::are_equal(test_argh[1], std::string("2"),
                      "Option test: positional test [1]");
    assert::are_equal(test_argh[2], std::string(),
                      "Option test: positional test [2]");
}

/**
 * @brief Subscript operator test.
 * @details Ensures that the subscript ([]) operator correctly counts the number
 *     of occurrences of a given argument.
 *
 */
void subscript_operator_test() {
    std::cout << "Running subscript operator test." << std::endl << std::endl;

    char_arr_arr test_args = {"program", "-1", "2", "3", "3"};
    argh test_argh(test_args.argc, test_args.argv);

    assert::are_equal(test_argh["0"], (size_t)0, "Subscript operator test [0]");
    assert::are_equal(test_argh["program"], (size_t)1,
                      "Subscript operator test [1]");
    assert::are_equal(test_argh["-1"], (size_t)1,
                      "Subscript operator test [2]");
    assert::are_equal(test_argh["2"], (size_t)1, "Subscript operator test [2]");
    assert::are_equal(test_argh["3"], (size_t)2, "Subscript operator test [3]");
}

/**
 * @brief Option-value test.
 * @details Ensures that an option may take a value, and that the rest of the
 *     `argh` instance is updated accordingly.
 *
 */
void option_value_test() {
    std::cout << "Running option-value test." << std::endl << std::endl;

    char_arr_arr test_args = {"program", "-1", "2", "--long", "value", "-3"};
    argh test_argh(test_args.argc, test_args.argv);

    assert::are_equal(test_argh.size(), (size_t)3,
                      "Option-value test: size() [3]");

    assert::are_equal(
        test_argh["2"], (size_t)1,
        "Option-value test: subscript operator[\"2\"] (before removal)");

    assert::are_equal(test_argh("-1"), std::string("2"),
                      "Option-value test: parentheses operator(\"-1\")");

    assert::are_equal(
        test_argh["2"], (size_t)0,
        "Option-value test: subscript operator[\"2\"] (after removal)");

    assert::are_equal(test_argh.size(), (size_t)2,
                      "Option-value test: size() [2a]");

    assert::are_equal(test_argh("-3"), std::string(""),
                      "Option-value test: parentheses operator(\"-3\")");

    assert::are_equal(test_argh.size(), (size_t)2,
                      "Option-value test: size() [2b]");

    assert::are_equal(
        test_argh("-nonexistent"), std::string(""),
        "Option-value test: parentheses operator(\"-nonexistent\")");

    assert::are_equal(test_argh.size(), (size_t)2,
                      "Option-value test: size() [2c]");

    assert::are_equal(
        test_argh["value"], (size_t)1,
        "Option-value test: subscript operator[\"value\"] (before removal)");

    assert::are_equal(test_argh("--long"), std::string("value"),
                      "Option-value test: parentheses operator(\"--long\")");

    assert::are_equal(
        test_argh["value"], (size_t)0,
        "Option-value test: subscript operator[\"value\"] (after removal)");

    assert::are_equal(test_argh.size(), (size_t)1,
                      "Option-value test: size() [1]");
}

void accepts_argument_test() {
    std::cout << "Running accepts_argument() test." << std::endl << std::endl;

    char_arr_arr test_args = {"program", "--option", "value", "3"};
    argh test_argh(test_args.argc, test_args.argv);

    assert::are_equal(test_argh.size(), (size_t)3,
                      "accepts_argument() test: size() [3a].");

    test_argh.accepts_argument("-nonexistent");

    assert::are_equal(test_argh.size(), (size_t)3,
                      "accepts_argument() test: size() [3b].");

    test_argh.accepts_argument("--option");

    assert::are_equal(test_argh.size(), (size_t)2,
                      "accepts_argument() test: size() [2a].");

    test_argh.accepts_argument("--option");

    assert::are_equal(test_argh.size(), (size_t)2,
                      "accepts_argument() test: size() [2b].");
}

/**
 * @brief Main program entry point.
 *
 * @param argc
 * @param argv
 * @return 0 on success, 1 on error.
 */
int main(int, char **) {
    simple_test();
    option_test();
    subscript_operator_test();
    option_value_test();
    accepts_argument_test();

    std::cout << std::endl << "All tests passed!" << std::endl;

    return 0;
}
