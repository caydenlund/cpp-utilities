/**
 * @file args.tests.cpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief Defines unit tests for the `util::args` class for parsing command-line arguments.
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "args.hpp"
#include "testing/testing.hpp"

using testing::run_all, testing::assert_eq, testing::assert_throws;
using util::args;


/*
    =========================
    ||  HELPER FUNCTIONS:  ||
    =========================
 */

/**
 * @brief Runs and checks a single `util::args` instance.
 * @details Creates a new `util::args` instance, calls `util::args#parse` on the given argument vector,
 *     and comprehensively checks the result for accuracy.
 *
 *     Example usage:
 *
 *     ```
 *     const std::string test_name = "Multi-option";
 *     const std::vector<char const*> argv {
 *         "exec",
 *         "-vvv",
 *         "-abc",
 *         "input.txt",
 *         "-n", "9",
 *         "--output", "file.txt"
 *     };
 *     const std::vector<std::string> operands { "input.txt" };
 *     const std::vector<std::tuple<std::string, unsigned int>> option_counts {
 *         { "-a", 1 },
 *         { "-b", 1 },
 *         { "-c", 1 },
 *         { "-n", 1 },
 *         { "-v", 3 },
 *         { "--output", 1 }
 *     };
 *     const std::vector<std::tuple<std::string, std::vector<std::string>>> option_args {
 *         {"-n", {"1"}},
 *         {"--output", {"file.txt"}}
 *     };
 *     run_parser_config(test_name, argv, operands, option_counts, option_args);
 *     ```
 *
 * @param test_name The name of the test (for error messages).
 * @param argv The argument vector (i.e., mock command-line arguments).
 * @param operands The non-option arguments provided to the command.
 * @param option_counts The number of times each option appeared in the argument vector.
 * @param option_args For each option that accepts arguments, the vector of option-arguments.
 */
void run_parser_config(const std::string& test_name, const std::vector<char const*>& argv,
                       const std::vector<std::string>& operands,
                       const std::unordered_map<std::string, unsigned int>& option_counts,
                       const std::unordered_map<std::string, std::vector<std::string>>& option_args) {
    /**
     * @brief Generates the error message for a particular assertion.
     *
     * @param method_name The name of the method under examination.
     * @param task The task being performed.
     * @return The generated error message.
     */
    const auto get_error_message = [&test_name](const std::string& method_name, const std::string& task) {
        return test_name + " `args#" + method_name + "` should " + task;
    };

    /**
     * @brief Checks two vectors for equality.
     *
     * @param method_name The name of the method under examination.
     * @param vector_name The name of the vector under examination.
     * @param left_vector The first vector to compare.
     * @param right_vector The second vector to compare.
     */
    const auto check_vectors = [&get_error_message](const std::string& method_name, const std::string& vector_name,
                                                    const std::vector<std::string>& left_vector,
                                                    const std::vector<std::string>& right_vector) {
        assert_eq(left_vector.size(), right_vector.size(),
                  get_error_message(method_name, "have the right number of " + vector_name));

        for (size_t index = 0; index < left_vector.size(); ++index) {
            assert_eq(left_vector[index], right_vector[index],
                      get_error_message(method_name, "report the right " + vector_name));
        }
    };

    //  Build a list of all options that accept arguments.
    std::vector<std::string> options_with_arguments;
    for (const auto& option : option_args) options_with_arguments.emplace_back(std::get<0>(option));

    //  Instantiate the parser and parse the argument vector.
    args parser(options_with_arguments);
    parser.parse(argv.size(), argv.data());

    //  Make sure that the program name was correctly recorded.
    if (!argv.empty()) {
        assert_eq(parser.name(), {argv[0]}, get_error_message("name()", "report the correct program name"));
    }

    //  Check that the correct operands are recorded (both in the iterator and through the subscript operator[]).
    const std::vector<std::string> actual_operands_it {parser.begin(), parser.end()};
    check_vectors("iterator", "operands", actual_operands_it, operands);
    std::vector<std::string> actual_operands_sub;
    for (unsigned int index = 0; index < (unsigned int)parser.size(); ++index) {  //  NOLINT(*-loop-convert)
        actual_operands_sub.emplace_back(parser[index]);
    }
    check_vectors("operator[int]", "operands", actual_operands_sub, operands);

    //  Check that the correct option counts are recorded.
    for (const auto& option_pair : option_counts) {
        const std::string& option_name = option_pair.first;
        const unsigned int option_count = option_pair.second;
        assert_eq(option_count, parser[option_name],
                  get_error_message("operator[string]", "report the correct count of option `" + option_name + "`"));
    }

    //  Check that the correct option-arguments are recorded.
    for (const auto& option_pair : option_args) {
        const std::string& option_name = option_pair.first;
        const std::vector<std::string>& expected_opt_args = option_pair.second;
        check_vectors("operator(string)", "opt-args of option `" + option_name + "`", expected_opt_args,
                      parser(option_name));
    }
}


/*
    ===================
    ||  TEST CASES:  ||
    ===================
 */

/**
 * @brief Ensures that the `args::args` constructor never raises an exception.
 *
 */
void test_constructor_no_fail() {
    const args parser_empty;

    const args parser_short_single({"-o"});
    const args parser_short_multi({"-a", "-b", "-c"});
    const args parser_short_malformed({"-abc", "-", "-_", "-=", "-)"});

    const args parser_long_single({"--output"});
    const args parser_long_multi({"--test_a", "--test_b", "--test_c"});
    const args parser_long_malformed({"--", "--_", "--=", "--)"});

    const args parser_misc_malformed({"", "_", "=", "o", "abc"});
}

/**
 * @brief Ensures that the `args::parse` method never raises an exception.
 *
 */
void test_parse_no_fail() {
    using argv_t = const std::vector<char const*>;

    args empty_parser;
    argv_t empty_argv;
    empty_parser.parse(empty_argv.size(), empty_argv.data());

    args spaces_parser;
    argv_t spaces_argv {"", " ", ""};
    spaces_parser.parse(spaces_argv.size(), spaces_argv.data());

    args symbols_parser;
    argv_t symbols_argv {"exec", "@", "-@", "--@"};
    symbols_parser.parse(symbols_argv.size(), symbols_argv.data());

    args hyphens_parser;
    argv_t hyphens_argv {"exec", "-", "--", "---", "-a", "--a", "-@", "--@", "-a=", "-a=b", "--a=", "--a=b"};
    hyphens_parser.parse(hyphens_argv.size(), hyphens_argv.data());

    args opts_parser({"-a", "--b", "-@", "--#", "=", "-=", "--="});
    argv_t opts_argv {"exec", "a",  "-a",  "--a", "b",  "-b",  "--b", "@",   "-@",  "--@",
                      "#",    "-#", "--#", "=",   "-=", "--=", "=c",  "-=c", "--=c"};
    opts_parser.parse(opts_argv.size(), opts_argv.data());
}

/**
 * @brief Ensures that `args::parse` correctly parses an argument vector with no arguments.
 *
 */
void test_parse_no_args() {
    run_parser_config("Empty argv", {}, {}, {}, {});
    run_parser_config("Empty program name", {""}, {}, {}, {});
    run_parser_config("Just program name", {"exec"}, {}, {}, {});
}

/**
 * @brief Ensures that `args::parse` correctly parses an argument vector with operands but not options.
 *
 */
void test_parse_operands() {
    run_parser_config("Simple argv", {"exec", "arg_1", "arg_2", "arg_3"}, {"arg_1", "arg_2", "arg_3"}, {}, {});
    run_parser_config("Reverse argv", {"exec", "arg_3", "arg_2", "arg_1"}, {"arg_3", "arg_2", "arg_1"}, {}, {});
    run_parser_config("Special operands",
                      {"exec", "=", "-=", "--=", "@", "-@", "--@", "a@", "-a@", "--a@", " ", "-", "---"},
                      {"=", "-=", "--=", "@", "-@", "--@", "a@", "-a@", "--a@", " ", "-", "---"}, {}, {});
    run_parser_config("Double-hyphen", {"exec", "--", "-o", "--output"}, {"-o", "--output"}, {}, {});
}

/**
 * @brief Ensures that `args::parse` correctly parses an argument vector with short options, but no option-arguments.
 *
 */
void test_parse_short_options_no_arg() {
    run_parser_config("Split options", {"exec", "-a", "-b", "-a", "-b", "-3", "-3"}, {},
                      {{"-a", 2}, {"-b", 2}, {"-3", 2}}, {});
    run_parser_config("Combined options", {"exec", "-aab3b3"}, {}, {{"-a", 2}, {"-b", 2}, {"-3", 2}}, {});
    run_parser_config("Both split and combined", {"exec", "-aab", "-3b3"}, {}, {{"-a", 2}, {"-b", 2}, {"-3", 2}}, {});
    run_parser_config("With operands", {"exec", "x", "-aab", "y", "-3b3", "z"}, {"x", "y", "z"},
                      {{"-a", 2}, {"-b", 2}, {"-3", 2}}, {});
}

/**
 * @brief Ensures that `args::parse` correctly parses an argument vector with short options and option-arguments.
 *
 */
void test_parse_short_options_with_arg() {
    run_parser_config("Split opt-args", {"exec", "-a", "x", "-b", "y", "-3", "z", "-a", "-a", "-b", "-b", "-3", "-3"},
                      {}, {{"-a", 2}, {"-b", 2}, {"-3", 2}},
                      {{"-a", {"x", "-a"}}, {"-b", {"y", "-b"}}, {"-3", {"z", "-3"}}});
    run_parser_config("Combined opt-args", {"exec", "-ax", "-by", "-3z", "-aa", "-bb", "-33"}, {},
                      {{"-a", 2}, {"-b", 2}, {"-3", 2}}, {{"-a", {"x", "a"}}, {"-b", {"y", "b"}}, {"-3", {"z", "3"}}});
    run_parser_config("Equals opt-args", {"exec", "-a=x", "-b=y", "-3=z", "-a=-a", "-b=-b", "-3=-3"}, {},
                      {{"-a", 2}, {"-b", 2}, {"-3", 2}},
                      {{"-a", {"x", "-a"}}, {"-b", {"y", "-b"}}, {"-3", {"z", "-3"}}});
    run_parser_config("Multi opt-args",
                      {"exec", "-a=x", "-b=y", "-3=z", "-3x", "-bz", "-ay", "-a", "z", "-b", "x", "-3", "y"}, {},
                      {{"-a", 3}, {"-b", 3}, {"-3", 3}},
                      {{"-a", {"x", "y", "z"}}, {"-b", {"y", "z", "x"}}, {"-3", {"z", "x", "y"}}});
    run_parser_config("With operands", {"exec", "7", "-ax", "8", "-by", "9", "-3z", "0"}, {"7", "8", "9", "0"},
                      {{"-a", 1}, {"-b", 1}, {"-3", 1}}, {{"-a", {"x"}}, {"-b", {"y"}}, {"-3", {"z"}}});
}

/**
 * @brief Ensures that `args::parse` correctly parses an argument vector with long options, but no option-arguments.
 *
 */
void test_parse_long_options_no_arg() {
    run_parser_config("Split options", {"exec", "--a", "--arg2", "--3"}, {}, {{"--a", 1}, {"--arg2", 1}, {"--3", 1}},
                      {});
    run_parser_config("With operands", {"exec", "7", "--a", "8", "--arg2", "9", "--3", "0"}, {"7", "8", "9", "0"},
                      {{"--a", 1}, {"--arg2", 1}, {"--3", 1}}, {});
}

/**
 * @brief Ensures that `args::parse` correctly parses an argument vector with long options and option-arguments.
 *
 */
void test_parse_long_options_with_arg() {
    run_parser_config("Split opt-args",
                      {"exec", "--a", "x", "--a", "--a", "--arg2", "y", "--arg2", "--arg2", "--3", "z", "--3", "--3"},
                      {}, {{"--a", 2}, {"--arg2", 2}, {"--3", 2}},
                      {{"--a", {"x", "--a"}}, {"--arg2", {"y", "--arg2"}}, {"--3", {"z", "--3"}}});
    run_parser_config("Equals opt-args", {"exec", "--a=x", "--a=--a", "--arg2=y", "--arg2=--arg2", "--3=z", "--3=--3"},
                      {}, {{"--a", 2}, {"--arg2", 2}, {"--3", 2}},
                      {{"--a", {"x", "--a"}}, {"--arg2", {"y", "--arg2"}}, {"--3", {"z", "--3"}}});
    run_parser_config("Multi opt-args", {"exec", "--a=x", "--arg2=y", "--3=z", "--3", "x", "--arg2", "z", "--a", "y"},
                      {}, {{"--a", 2}, {"--arg2", 2}, {"--3", 2}},
                      {{"--a", {"x", "y"}}, {"--arg2", {"y", "z"}}, {"--3", {"z", "x"}}});
    run_parser_config("With operands", {"exec", "7", "--a", "x", "8", "--arg2=y", "9", "--3", "z", "0"},
                      {"7", "8", "9", "0"}, {{"--a", 1}, {"--arg2", 1}, {"--3", 1}},
                      {{"--a", {"x"}}, {"--arg2", {"y"}}, {"--3", {"z"}}});
}

/**
 * @brief Main program entry point.
 *
 * @return 0.
 */
int main() {
    const std::vector<std::tuple<std::function<void()>, std::string>> all_tests = {
            {test_constructor_no_fail, "`args::args` no-fail"},
            {test_parse_no_fail, "`args::parse` no-fail"},
            {test_parse_no_args, "`args::parse` with no arguments"},
            {test_parse_operands, "`args::parse` with only operands"},
            {test_parse_short_options_no_arg, "`args::parse` with short options, no opt-args"},
            {test_parse_short_options_with_arg, "`args::parse` with short options and opt-args"},
            {test_parse_long_options_no_arg, "`args::parse` with long options, no opt-args"},
            {test_parse_long_options_with_arg, "`args::parse` with long options and opt-args"},
    };
    run_all(all_tests);

    return 0;
}
