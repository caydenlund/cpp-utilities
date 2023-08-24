/**
 * @file args.hpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief Defines the `util::args` class, which parses command-line arguments.
 * @details The arguments are inspired by the POSIX and GNU argument syntax, according to the following rules:
 *
 *     * There are four types of arguments:
 *         1. **The program name**.
 *            This is, of course, the first argument in the array.
 *            In a call to `clang++ file.cpp -o exec`, the program name is `clang++`.
 *
 *         2. **Options**.
 *            There are two kinds of options: short (single-character) and long (multi-character).
 *            Short options are preceded by a single hyphen, and long options are preceded by two hyphens.
 *            In a call to `clang++ file.cpp -o exec`, `-o` is a short option.
 *            Short options may only be made of letters (uppercase or lowercase) or numbers.
 *            Long options may only be made of letters, numbers, hyphens, and underscores;
 *            they may not start with a hyphen. Options are case-sensitive.
 *
 *         3. **Option-arguments**.
 *            These are arguments of an option.
 *            In a call to `clang++ file.cpp -o exec`, `exec` is an option-argument
 *            (i.e., the argument of the option `-o`).
 *
 *         4. **Operands**.
 *            These are arguments of the main program (not arguments of an option).
 *            In a call to `clang++ file.cpp -o exec`, `file.cpp` is an operand.
 *
 *         5. **Double-hyphen**.
 *            This is a special kind of argument that doesn't fit into the above categories.
 *            All following arguments are operands, even if they start with hyphens.
 *            In a call to `./exec -a -b -c -- -d -e -f`, `-d`, `-e`, and `-f` are operands, not options.
 *
 *     * Short options are always preceded by a single hyphen. (E.g., `./exec -a -b -c`.)
 *
 *     * Short options may be combined.
 *       Note that the only short option that can accept an option-argument is the final option.
 *       (E.g., `./exec -a -b -c` is equivalent to `./exec -abc`. `-a` and `-b` cannot accept option-arguments.)
 *
 *     * There are three ways to specify option-arguments:
 *         1. For short options only, the option-argument may be specified directly after the option, with no delimiter.
 *            In a call to `./exec -n5`, `5` is the option-argument of `-n`.
 *
 *         2. The option-argument may be specified with a space between the option and the option-argument.
 *            In a call to `./exec --number 5`, `5` is the option-argument of `--number`.
 *            Similarly, in a call to `./exec -an 5`, `5` is the option-argument of `-n`.
 *
 *         3. The option-argument may be specified with the equals character between the option and the option-argument.
 *            In a call to `./exec --number=5`, `5` is the option-argument of `--number`.
 *            Similarly, in a call to `./exec -an=5`, `5` is the option-argument of `-n`.
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#ifndef ARGS_HPP
#define ARGS_HPP

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace util {

    /**
     * @brief Defines the `args` class for parsing command line arguments.
     *
     * @details Usage is simple.
     *     First, instantiate the object with a list of all options that take option-arguments.
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         const std::vector<std::string> options_with_args {
     *             "-n", "--number",
     *             "-i", "--input"
     *         };
     *
     *         util::args parser(options_with_args);
     *
     *         //  . . .
     *     }
     *     ```
     *
     *     Next, use it to parse the argument array.
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         //  . . .
     *
     *         parser.parse(argc, argv);
     *
     *         //  . . .
     *     }
     *     ```
     *
     *     After that, you can count the number of times an option has been specified
     *     (or check for the presence of the option) using the subscript `[]` operator.
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         //  . . .
     *
     *         if (parser["-h"] || parser["--help"]) {
     *             print_help_message();
     *             return 0;
     *         }
     *
     *         std::cout << "The option `-n` has been used " << parser["-n"] << " times." << std::endl;
     *
     *         //  . . .
     *     }
     *     ```
     *
     *     You can get a vector of all the option-arguments used for a given option with
     *     the function call `()` operator.
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         //  . . .
     *
     *         if (parser["-n"]) {
     *             //  If the program has been called as `./exec -n 1 -n 2 -n 3`,
     *             //  then this will return the vector `{"1", "2", "3"}`.
     *             const std::vector<std::string> all_numbers = parser("-n");
     *
     *             for (const std::string& number : all_numbers) {
     *                 std::cout << number << std::endl;
     *             }
     *         }
     *
     *         //  . . .
     *     }
     *     ```
     *
     *     Access the operands using the subscript `[]` operator with an integer index.
     *     Note that subscript zero returns the first operand, not the program name.
     *     You can also use the `size()` method to get the number of operands plus one (for the program name).
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         //  . . .
     *
     *         for (unsigned int arg_index = 0; arg_index < parser.size(); ++arg_index) {
     *             std::cout << "Arg " << arg_index << ": " << parser[arg_index] << std::endl;
     *         }
     *
     *         //  . . .
     *     }
     *     ```
     *
     *     You can also use the `begin()` and `end()` methods to iterate over the operands.
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         //  . . .
     *
     *         const std::vector<std::string> operands(parser.begin(), parser.end());
     *
     *         for (const std::string& operand : parser) {
     *             std::cout << "Operand: " << operand << std::endl;
     *         }
     *
     *         //  . . .
     *     }
     *     ```
     *
     *     The `name()` method returns the program name.
     *
     *     ```
     *     int main(int argc, char** argv) {
     *         //  . . .
     *
     *         const std::string program_name = parser.name();
     *
     *         //  . . .
     *     }
     *     ```
     */
    class args {
    private:
        /**
         * @brief The name of the program.
         *
         */
        std::string _name;

        /**
         * @brief The set of all operands in the call arguments.
         *
         */
        std::vector<std::string> _operands;

        /**
         * @brief A mapping from option names to option-arguments.
         *
         */
        std::unordered_map<std::string, std::vector<std::string>> _option_arguments;

        /**
         * @brief A mapping from option names to the count of occurrences.
         *
         */
        std::unordered_map<std::string, unsigned int> _option_counts;

        /**
         * @brief The set of all option names that accept arguments.
         *
         */
        std::unordered_set<std::string> _options_with_arguments;

    public:
        /**
         * @brief Class constructor.
         *
         * @param options_with_arguments A list of all options that take option-arguments. Defaults to an empty list.
         */
        args(const std::vector<std::string>& options_with_arguments = {})
            : _options_with_arguments(options_with_arguments.begin(), options_with_arguments.end()) {}

        /**
         * @brief Subscript `[]` operator.
         * @details Returns the operand at the given index.
         *     Throws a `std::runtime_error` on an invalid index.
         *
         * @param index The index of the operand to return.
         * @return The operand at the given index.
         */
        std::string operator[](unsigned int index) const {
            if (index > _operands.size()) throw std::runtime_error("Out-of-bounds argument access");
            return _operands[index];
        }

        /**
         * @brief Subscript `[]` operator.
         * @details Returns the count of occurrences of the given option.
         *
         * @param option The option of which to count occurrences.
         * @return The count of occurrences of the given option.
         */
        unsigned int operator[](const std::string& option) const {
            return _option_counts.contains(option) ? _option_counts.at(option) : 0;
        }

        /**
         * @brief Function call `()` operator.
         * @details Returns the list of option-arguments for the given option.
         *
         * @param option The option for which to list option-arguments.
         * @return The list of option-arguments for the given option.
         */
        std::vector<std::string> operator()(const std::string& option) const {
            return _option_arguments.contains(option) ? _option_arguments.at(option) : std::vector<std::string>();
        }

        /**
         * @brief Iterator `begin()`.
         * @details Returns the start of the iterator for the list of operands.
         *
         * @return The start of the iterator for the list of operands.
         */
        std::vector<std::string>::const_iterator begin() const { return _operands.cbegin(); }

        /**
         * @brief Iterator `end()`.
         * @details Returns the end of the iterator for the list of operands.
         *
         * @return The end of the iterator for the list of operands.
         */
        std::vector<std::string>::const_iterator end() const { return _operands.cend(); }

        /**
         * @brief Parses the given arguments.
         *
         * @param argc The number of command-line arguments.
         * @param argv The array of command-line arguments.
         */
        void parse(unsigned int argc, char const* const* argv) {
            //  Given a character `'X'`, returns a string of that character as a short option (`"-X"`).
            const auto char_as_opt = [](char option) -> std::string { return "-" + std::string {option}; };

            //  Reports whether the given character is alphanumeric.
            const auto char_is_alphanumeric = [](char character) -> bool {
                return (('a' <= character && character <= 'z') || ('A' <= character && character <= 'Z')
                        || ('0' <= character && character <= '9'));
            };

            //  Record the program name.
            if (argc > 0) _name = argv[0];

            //  If we come across the argument `--`, then all remaining arguments are program operands.
            bool found_double_hyphen = false;

            //  Iterate over remaining arguments in `argv`.
            for (unsigned int index = 1; index < argc; ++index) {
                //  The current argument under examination.
                const std::string argument = argv[index];

                //  If we've found a double-hyphen, then this is an operand.
                //  A single hyphen and the empty string are always an operand.
                //  If the argument doesn't start with a hyphen, then this is an operand.
                if (found_double_hyphen || argument == "-" || argument.empty() || argument[0] != '-') {
                    _operands.emplace_back(argument);
                    continue;
                }

                //  If this is a double-hyphen, then all following arguments are operands.
                if (argument == "--") {
                    found_double_hyphen = true;
                    continue;
                }

                //  If we've made it this far, then we know that three things are true:
                //    1. This argument starts with a hyphen.
                //    2. A double-hyphen has not been encountered.
                //    3. This argument is more than just a single-hyphen or a double-hyphen operand.
                //  That means that this argument is an option, unless it is malformed.

                //  First, handle long options.
                //  Long options match the regular expression `--[0-9A-Za-z_]+[0-9A-Za-z_\-]*(=.*)?`.
                //    * They start with two hyphens.
                //    * Next, they need a letter, number, or underscore.
                //    * After that, they need any number of letters, numbers, underscores, or hyphens.
                //    * Finally, if this option accepts option-arguments, then there may be an equals sign.
                //      All characters following the equals sign are the option-argument of this option.
                //  If an equals sign is found, and the preceding string is not an option that accepts
                //  option-arguments, then this is not a long option (and is therefore an operand).
                if (argument[1] == '-') {
                    //  We scan through the string to check for validity.
                    enum scanner_state { START, VALID_SO_FAR, FOUND_EQUALS, VALID, INVALID };
                    scanner_state state = START;
                    unsigned int current_index = 2;

                    while (true) {
                        const bool reached_end = current_index >= argument.size();
                        const char current_char = reached_end ? '\0' : argument[current_index];
                        const bool current_char_is_valid = reached_end || char_is_alphanumeric(current_char);

                        switch (state) {
                            case START:
                                //  This is the first iteration. Make sure that the first character is valid.
                                state = (current_char_is_valid || current_char == '_') ? VALID_SO_FAR : INVALID;
                                ++current_index;
                                continue;

                            case VALID_SO_FAR:
                                //  Everything up to this point has been a valid option name.
                                //  If we've reached the end of the string, then we're done.
                                //  Otherwise, make sure that this character is valid.
                                if (reached_end) {
                                    state = VALID;
                                    continue;
                                }

                                ++current_index;

                                if (current_char == '=') {
                                    state = FOUND_EQUALS;
                                    continue;
                                }

                                if (!current_char_is_valid && current_char != '_' && current_char != '-')
                                    state = INVALID;
                                continue;

                            case FOUND_EQUALS: {
                                //  Everything up to this point has been a valid option name, and we have found `'='`.
                                //  Make sure that it's an option that can accept an argument, and handle it.
                                const std::string option = argument.substr(0, current_index - 1);
                                if (!_options_with_arguments.contains(option)) {
                                    state = INVALID;
                                    continue;
                                }

                                const std::string value = reached_end ? "" : argument.substr(current_index);
                                if (_option_arguments.contains(option)) {
                                    _option_arguments[option].emplace_back(value);
                                } else {
                                    _option_arguments[option] = {value};
                                }

                                if (_option_counts.contains(option)) {
                                    ++_option_counts[option];
                                } else {
                                    _option_counts[option] = 1;
                                }

                                break;
                            }

                            case VALID:
                                //  This is a valid long option.
                                //  If this option accepts an argument,
                                //  then we need to get the next argument and save it.
                                //  We also need to update the count of this option.
                                if (_options_with_arguments.contains(argument)) {
                                    const std::string value = (++index == argc) ? "" : argv[index];
                                    if (_option_arguments.contains(argument)) {
                                        _option_arguments[argument].emplace_back(value);
                                    } else {
                                        _option_arguments[argument] = {value};
                                    }
                                }

                                if (_option_counts.contains(argument)) {
                                    ++_option_counts[argument];
                                } else {
                                    _option_counts[argument] = 1;
                                }
                                break;

                            case INVALID:
                                //  This is not a valid long option.
                                //  Add the argument to the list of operands and break out of the loop.
                                _operands.emplace_back(argument);
                                break;
                        }

                        //  If we've made it this far, break out of the loop.
                        break;
                    }

                    //  We've handled this argument. Continue to the next one.
                    continue;
                }

                //  If we've made it this far, then one additional thing is true:
                //  this argument starts with a single hyphen, but not two.
                //  That means that this is a short option, unless it is malformed.
                //  Short options match the regular expression `-[0-9A-Za-z]+(=.*)?`.
                //    * They start with one hyphen.
                //    * They are then a single letter or number.
                //    * They may also be combined with any number of other letters and numbers.
                //    * If it contains a short option that accepts option-arguments, then there may be an equals sign.
                //      All characters following the equals sign are the option-argument of this option.
                //      If there is not an equals sign, then all characters following the short option are the
                //      option-argument of this option.
                //  If an equals sign is found, and the preceding string is not an option that accepts
                //  option-arguments, then this is not a short option (and is therefore an operand).
                //  We scan through the string to check for validity.
                enum scanner_state { VALID_SO_FAR, FOUND_OPT_ARG, VALID, INVALID };
                scanner_state state = VALID_SO_FAR;
                unsigned int current_index = 1;

                //  Given a string, adds one to the count of each short option in the string.
                //  Don't feed it hyphens or option-arguments.
                const auto add_all = [&](const std::string& input) {
                    for (const char& character : input) {
                        const std::string option = char_as_opt(character);
                        if (_option_counts.contains(option)) {
                            ++_option_counts[option];
                        } else {
                            _option_counts[option] = 1;
                        }
                    }
                };

                while (true) {
                    const bool reached_end = current_index >= argument.size();
                    const char current_char = reached_end ? '\0' : argument[current_index];
                    const bool current_char_is_valid = reached_end || char_is_alphanumeric(current_char);

                    switch (state) {
                        case VALID_SO_FAR:
                            //  This is the starting state.
                            //  The scanner stays in this state until it encounters the end of the string,
                            //  an invalid character, or an option that accepts option-arguments.
                            if (reached_end) {
                                state = VALID;
                                continue;
                            }

                            if (!current_char_is_valid) {
                                state = INVALID;
                                continue;
                            }

                            if (_options_with_arguments.contains(char_as_opt(current_char))) {
                                state = FOUND_OPT_ARG;
                                continue;
                            }

                            ++current_index;
                            continue;

                        case FOUND_OPT_ARG: {
                            //  The short option at the current index accepts an option-argument.
                            //  If there are more characters in the argument, then they are the option-argument.
                            //  Otherwise, the next argument is the option-argument.
                            const std::string option = char_as_opt(current_char);
                            std::string value;
                            if (current_index + 1 < argument.size()) {
                                if (argument[current_index + 1] == '=') {
                                    value = (current_index + 2 < argument.size()) ? argument.substr(current_index + 2)
                                                                                  : "";
                                } else {
                                    value = argument.substr(current_index + 1);
                                }
                            } else {
                                value = (index + 1 < argc) ? argv[++index] : "";
                            }

                            if (_option_arguments.contains(option)) {
                                _option_arguments[option].emplace_back(value);
                            } else {
                                _option_arguments[option] = {value};
                            }

                            add_all(argument.substr(1, current_index));

                            break;
                        }

                        case VALID:
                            //  This is a valid short option, and it doesn't accept an argument.
                            //  Add one to the counts of all the short options.
                            add_all(argument.substr(1));
                            break;

                        case INVALID:
                            //  This is not a valid long option.
                            //  Add the argument to the list of operands and break out of the loop.
                            _operands.emplace_back(argument);
                            break;
                    }

                    //  If we've made it this far, break out of the loop.
                    break;
                }

                //  We've handled this argument. Continue to the next one.
            }
        }

        /**
         * @brief Reports the number of operands.
         *
         * @return The number of operands.
         */
        [[nodiscard]] size_t size() const { return _operands.size(); }

        /**
         * @brief Reports the name of the program.
         *
         * @return The name of the program.
         */
        [[nodiscard]] std::string name() const { return _name; }
    };

}  //  namespace util

#endif  //  ARGS_HPP
