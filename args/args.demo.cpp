/**
 * @file args.demo.cpp
 * @brief A demonstration of the `util::args` utility.
 * @details Pass any of the following options:
 *     * `-n <num>  |  --number <num>`
 *     * `-o <str>  |  --output <str>`
 *     * `-h        |  --help`
 *     * `-v`
 */

import args;

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Main program entry point.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on success.
 */
int main(int argc, char** argv) {
    //  First, construct a `util::args` object with a list of all options
    //  that accept arguments.
    const std::vector<std::string> options_with_args {"-n", "--number", "-o", "--output"};
    util::args parser(options_with_args);

    //  Next, pass the command-line arguments to the argument parser.
    parser.parse(argc, argv);
    std::cout << "Program name: `" << parser.name() << "`.\n";

    //  Access the number of times an argument was given using the square brackets.
    if (parser["-h"] + parser["--help"] > 0) std::cout << "User passed `-h` or `--help`.\n";
    std::cout << "Verbosity level: " << parser["-v"] << "\n";

    //  Get the value of an option-argument using parentheses (like a function call).
    if (parser["-o"] > 0) {
        for (const auto& arg : parser("-o")) std::cout << "User passed `-o " << arg << "`.\n";
    }
    if (parser["--output"] > 0) {
        for (const auto& arg : parser("--output")) std::cout << "User passed `--output " << arg << "`.\n";
    }
    if (parser["-n"] > 0) {
        for (const auto& arg : parser("-n")) std::cout << "User passed `-n " << arg << "`.\n";
    }
    if (parser["--number"] > 0) {
        for (const auto& arg : parser("--number")) std::cout << "User passed `--number " << arg << "`.\n";
    }

    return 0;
}
