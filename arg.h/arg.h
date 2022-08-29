/**
 * @file arg.h
 * @author Cayden Lund (cayden.lund@utah.edu)
 * @brief The header-only library to parse command line arguments.
 * @date 27 August 2022
 *
 * @copyright Copyright (c) 2022 by Cayden Lund
 */

#ifndef ARG_H
#define ARG_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//  //=====================\\
//  ||  Class definitions  ||
//  \\=====================//

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
 */
class argh {
   public:
    /**
     * @brief Constructs a new argh object.
     * @details Forwards a const-qualified argv array to the other constructor.
     *
     * @param argc The count of arguments.
     * @param argv The argument array.
     */
    argh(int argc, char **argv) : argh(argc, (char const **)argv) {}

    /**
     * @brief Constructs a new argh object.
     *
     * @param argc The count of arguments.
     * @param argv The argument array.
     */
    argh(int argc, char const **argv);

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
    size_t operator[](const std::string &name) const;

    /**
     * @brief Reports the (argument) value of the given option
     *     and tells argh that the option accepts an argument.
     * @details Example:
     *
     *     // Program executed as "find / -name fff".
     *     argh args(argc, argv);
     *
     *     args["fff"];  // Returns 1.
     *     args::size();  // Returns 3.
     *
     *     args("-name");  // Returns "fff".
     *
     *     args["fff"];  // Returns 0.
     *     args::size();  // Returns 2.
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
    std::string operator[](size_t index) const;

    /**
     * @brief Returns the number of non-option arguments.
     * @details Includes the program name.
     *
     * @return The number of non-option arguments.
     */
    size_t size() const;

   private:
    /**
     * @brief A mapping of option arguments to their corresponding values.
     *
     * @tparam std::string The name of the option argument.
     * @tparam size_t The unique ID of the option argument's value.
     */
    std::unordered_map<std::string, size_t> _option_value;

    /**
     * @brief A mapping of option arguments to the count of occurrences of
     *     that option.
     * @details (An option is an argument with a leading dash, not including
     *     the arguments "-" and "--".)
     *
     * @tparam std::string The name of the option argument.
     * @tparam size_t The number of times the option appears.
     */
    std::unordered_map<std::string, size_t> _option_count;

    /**
     * @brief A set of all options that accept arguments.
     *
     * @tparam std::string The name of the option argument.
     */
    std::unordered_set<std::string> _accepts_arguments;

    /**
     * @brief The list of non-option arguments.
     *
     * @tparam size_t The unique ID of the argument.
     */
    std::vector<size_t> _non_option_arguments;

    /**
     * @brief The list of all arguments.
     * @details The index of an argument in this list is its unique ID, used by
     *     the `_non_option_arguments` and other lists.
     *
     * @tparam std::string The name of the argument.
     */
    std::vector<std::string> _all_arguments;
};

//  //========================\\
//  ||  argh: public methods  ||
//  \\========================//

argh::argh(int argc, char const **argv) {
    for (size_t id = 0; id < argc; id++) {
        std::string argument(argv[id]);
        this->_all_arguments.emplace_back(argument);
        this->_option_count[argument]++;

        bool is_option = (!argument.empty() && argument[0] == '-' &&
                          argument != "-" && argument != "--");

        if (is_option) {
            if (id + 1 < argc) {
                this->_option_value[argument] = id + 1;
            }
        } else {
            this->_non_option_arguments.emplace_back(id);
        }
    }
}

void argh::accepts_argument(const std::string &name) {
    // Has the option already been marked as accepting an argument?
    // If so, return.
    if (this->_accepts_arguments.count(name) > 0) return;

    // Is it an option? If not, return.
    if (name[0] != '-' || name == "-" || name == "--") return;

    // Was the option in the program argument list? If not, return.
    if (this->_option_count[name] == 0) return;

    // Put the option in the `_accepts_arguments` set.
    this->_accepts_arguments.insert(name);

    // Scan through the non-option arguments until we find
    // the one that we need to remove.
    size_t target = this->_option_value[name];
    for (auto it = this->_non_option_arguments.begin();
         it != this->_non_option_arguments.end(); it++) {
        if (*it == target) {
            this->_non_option_arguments.erase(it);
            this->_option_count[this->_all_arguments[target]]--;
            return;
        }
    }
}

size_t argh::operator[](const std::string &name) const {
    if (this->_option_count.count(name) == 0) return 0;

    return this->_option_count.at(name);
}

std::string argh::operator()(const std::string &name) {
    // Is it an option? If not, return.
    if (name[0] != '-' || name == "-" || name == "--") return "";

    // Was the option in the program argument list? If not, return.
    if (this->_option_count[name] == 0) return "";

    // Was the option the last argument in the program argument list?
    // If so, return.
    if (this->_all_arguments[this->_all_arguments.size() - 1] == name)
        return "";

    // Indicate that the option accepts an argument.
    this->accepts_argument(name);

    // Return the argument value.
    return this->_all_arguments[this->_option_value[name]];
}

std::string argh::operator[](size_t index) const {
    if (index >= this->_non_option_arguments.size()) return "";

    return this->_all_arguments[this->_non_option_arguments[index]];
}

size_t argh::size() const { return this->_non_option_arguments.size(); }

#endif