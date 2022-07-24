/**
 * @file arg.h
 * @author Cayden Lund (cayden.lund@utah.edu)
 * @brief The header-only library to parse command line arguments.
 * @version 3.0
 *
 * @copyright Copyright (c) 2022 Cayden Lund
 *
 */

#ifndef ARG_H
#define ARG_H

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Use this class to parse command line arguments.
 * @details
 *     This class is used to parse command line arguments.
 *
 *     We use the following style of arguments
 *     (adapted from GNU's style guide):
 *
 *       * Arguments are options if they begin with a hyphen delimiter (`-`).
 *
 *       * Multiple options may follow a single hyphen delimiter in a single
 *         token if the options do not take arguments.
 *         Thus, `-abc` is equivalent to `-a -b -c`.
 *
 *       * Option names are single alphanumeric characters or begin with two
 *         hyphen delimiters.
 *
 *       * Certain options require an argument.
 *         For example, the -o option of the `ld` command requires an argument:
 *         an output file name.
 *
 *       * An option and its argument must appear as separate tokens.
 *         (In other words, the whitespace separating them is required.)
 *         Alternatively, the equals character (`=`) may be used.
 *         Thus, `-o foo` and `-o=foo` are equivalent.
 *
 *       * Options typically precede other non-option arguments.
 *
 *       * The argument `--` terminates all options; any following arguments are
 *         treated as non-option arguments, even if they begin with a hyphen.
 *
 *       * A token consisting of a single hyphen character is interpreted as an
 *         ordinary non-option argument.
 *         By convention, it is used to specify input from or output to
 *         the standard input and output streams.
 *
 *       * Options may be supplied in any order, or appear multiple times.
 *
 *       * Long options consist of `--` followed by a name made of alphanumeric
 *         characters and dashes.
 *         Option names are typically one to three words long, with hyphens to
 *         separate words. Users can abbreviate the option names as long as the
 *         abbreviations are unique.
 *
 *       * To specify an argument for a long option,
 *         write either `--name=value` or `--name value`.
 *
 *     This style of arguments deviates from GNU's style in the following ways:
 *
 *       * In GNU style, to specify an argument for a long option,
 *         write `--name=value`.
 *
 *         In argh, either `--name=value` or `--name value` is accepted.
 *
 *       * In GNU style, an option and its argument may or may not appear
 *         as separate tokens.
 *         (In other words, the whitespace separating them is optional.)
 *         Thus, `-t ar` and `-tar` are equivalent.
 *
 *         In argh, an option and its argument must appear as separate tokens.
 *         (In other words, the whitespace separating them is required.)
 *         Thus, `-tar` would be interpreted as `-t -a -r`,
 *         even if `-t` requires an argument.
 *
 *       * Some GNU programs require options to precede other arguments.
 *
 *         In argh, options may appear anywhere.
 *         In other words, `-o output.txt input.txt` and
 *         `input.txt -o output.txt` are equivalent.
 *
 *     Usage is quite simple.
 *
 *     1. Pass the argv array to the constructor:
 *
 *            argh args(argc, argv);
 *
 *     2. Access whether an option is present using the operator[]
 *        with a string.
 *        It returns an integer count of the number of occurrences of
 *        the option.
 *
 *            if (args["-h"] + args["--help"] > 0) {
 *                display_help_message();
 *                return 0;
 *            }
 *
 *            unsigned int verbosity_level = args["-v"] + args["--verbose"];
 *
 *     3. Access an option's value using the operator() with a string.
 *
 *            std::string output_file = args("-o");
 *
 *     4. Access non-option arguments by index using the operator[]
 *        with an integer.
 *
 *            std::string program_name = args[0];
 *            std::string input_file_name = args[1];
 *
 *        Note: argh can't on its own determine whether an argument belongs to
 *            an option or not.
 *            (E.g., in `program -q file.txt`,
 *            should `file.txt` belong to `-q` or not?)
 *            Thus, it initially assumes that all arguments do not belong to
 *            an option.
 *
 *            There are two ways to tell argh that an option takes an argument:
 *              1. Using the operator() (see item 3 above).
 *              2. Using the function accepts_argument() (see item 5 below).
 *
 *     5. Tell argh that an option takes an argument using accepts_argument()
 *        with a string.
 *
 *            args.accepts_argument("-o");
 *
 * TODO: Const references.
 */
class argh {
   public:
    /**
     * @brief Constructs a new argh object.
     *
     * @param argc The count of arguments.
     * @param argv The argument array.
     */
    argh(int argc, char **argv);

    /**
     * @brief Destroys the argh object.
     *
     */
    ~argh();

    /**
     * @brief Tells argh that the given option requires an argument.
     *
     * @param name The option that requires an argument.
     *             Leading dashes are required.
     */
    void accepts_argument(const std::string &name);

    /**
     * @brief Reports the number of times the given option occurs.
     *
     * @param name The option to count.
     *             Leading dashes are required.
     * @return The number of times the option occurs.
     */
    unsigned int operator[](const std::string &name);

    /**
     * @brief Reports the (argument) value of the given option
     *     and tells argh that the option accepts an argument.
     *
     * @param name The option to get the value of.
     *             Leading dashes are required.
     * @return The value of the option.
     */
    std::string operator()(const std::string &name);

    /**
     * @brief Accesses the non-option argument at the given index.
     *
     * @param index The index of the argument.
     * @return The value of the argument.
     */
    std::string operator[](unsigned int index);

    /**
     * @brief Returns the number of non-option arguments.
     *
     * @return The number of non-option arguments.
     */
    unsigned int size();

   private:
    /**
     * @brief The list of arguments.
     *
     */
    std::vector<std::string> _arguments;

    /**
     * @brief The list of non-option arguments.
     * @details Holds the indices of the arguments in the `_arguments` vector.
     *
     */
    std::vector<unsigned int> _non_option_arguments;
};

argh::argh(int argc, char **argv) {
    unsigned int id = 0;

    // Iterate through the arguments.
    for (int i = 0; i < argc; i++) {
        // Get the argument.
        argument arg;
        arg.name = argv[i];
        arg.id = id++;

        // Check whether the argument is just "-" or "--".
        if (arg.name == "-" || arg.name == "--") {
            // This is a non-option argument.
            _arguments.push_back(arg);
            _non_option_arguments.push_back(arg);
            continue;
        }

        // Check whether the argument starts with "-".
        if (arg.name[0] == '-') {
            // If so, it is an option.
            arg.is_option = true;

            // Check whether the argument is a long option.
            if (arg.name[1] == '-') {
                // Check whether the string contains "=".
                size_t equals_index = arg.name.find('=');
                if (equals_index != std::string::npos) {
                    arg.has_value = true;
                    arg.value = arg.name.substr(equals_index + 1);
                    arg.name = arg.name.substr(0, equals_index);
                }

                _arguments.push_back(arg);

                // If the option isn't in the count map, add it.
                if (this->_option_counts.find(arg.name) ==
                    this->_option_counts.end()) {
                    this->_option_counts[arg.name] = 0;
                }

                // Increment the count of the option.
                this->_option_counts[arg.name]++;

                continue;
            }

            // Otherwise, it's one or more short options.

            // Iterate through the characters in the argument.
            for (int j = 1; j < arg.name.size(); j++) {
                argument option;
                option.name = "-" + arg.name.substr(j, 1);
                option.id = id++;
                option.is_option = true;

                // If the option isn't in the count map, add it.
                if (this->_option_counts.find(option.name) ==
                    this->_option_counts.end()) {
                    this->_option_counts[option.name] = 0;
                }

                // Increment the count of the option.
                this->_option_counts[option.name]++;

                // And add the option to the list.
                this->_arguments.push_back(option);
            }

            continue;
        }

        // Otherwise, it's a non-option argument.
        _arguments.push_back(arg);
        _non_option_arguments.push_back(arg);
    }
}

void argh::accepts_argument(const std::string &name) {
    // Just calling the operator() with the argument name
    // will ensure that the option is marked as having a value.
    (*this)(name);
}

unsigned int argh::operator[](const std::string &name) {
    return this->_option_counts[name];
}

std::string argh::operator()(const std::string &name) {
    // If the argument isn't in the map, then we don't need to do anything.
    if (this->_option_counts[name] == 0) return "";

    // Iterate over the arguments until we find the one with the given name.
    auto it = this->_arguments.begin();
    while (it != this->_arguments.end()) {
        if (it->name == name) {
            // Once we find the argument, if it has a value, return it.
            if (it->has_value) return it->value;

            // Otherwise, we need to read the next argument and update this one.
            it->has_value = true;
            std::string &value = it->value;
            it++;

            // Ensure that the next argument has a value.
            value = "";
            if (it == this->_arguments.end()) return "";
            if (it->is_option) return "";

            // If the next argument is not an option,
            // then its name is the value.
            value = it->name;

            unsigned int id = it->id;

            // We need to remove the argument from the overall list.
            this->_arguments.erase(it);

            // We also need to remove it from the non-option list.
            auto non_option_it = this->_non_option_arguments.begin();
            while (non_option_it != this->_non_option_arguments.end()) {
                if (non_option_it->id == id) {
                    this->_non_option_arguments.erase(non_option_it);
                    break;
                }
                non_option_it++;
            }

            // Finally, we can return the value.
            return value;
        }
        it++;
    }
    return "";
}

std::string argh::operator[](unsigned int index) {
    if (index >= this->_non_option_arguments.size()) {
        return "";
    }

    auto it = this->_non_option_arguments.begin();
    std::advance(it, index);
    return it->name;
}

unsigned int argh::size() { return this->_non_option_arguments.size(); }

#endif
