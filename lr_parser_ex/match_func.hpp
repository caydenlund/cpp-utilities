/**
 * @file match_func.hpp
 * @brief Defines the `match_func` class.
 * @details Because this is a template class, implementation is in this file.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef MATCH_FUNC_HPP
#define MATCH_FUNC_HPP

//  STL includes:
#include <functional>
#include <string>
#include <utility>


/*
    ===================
    ||  Definition:  ||
    ===================
*/

/**
 * @brief A token-matching function.
 * @details Can be implicitly constructed from either an explicit token-matching function,
 *     or a static string or character.
 *
 */
class match_func {
public:
    /**
     * @brief The signature of a matching function.
     * @details Accepts a string input and an index into the string from which to lex.
     *     Returns the new index if the match was successful;
     *     otherwise, the same index is returned.
     *
     */
    using match_func_t = std::function<size_t(const std::string&, size_t)>;

    /**
     * @brief Class constructor.
     * @details Implicit constructor from an explicit matching function.
     *
     * @param func The matching function to be called.
     */
    match_func(const match_func_t& func);

    /**
     * @brief Class constructor.
     * @details Implicit constructor from a string.
     *
     * @param static_string The string token to be matched.
     */
    match_func(const std::string& static_string);

    /**
     * @brief Class constructor.
     * @details Implicit constructor from a character.
     *
     * @param static_char The character to be matched.
     */
    match_func(char static_char);

    /**
     * @brief Operator().
     *
     * @param input The input string to lex.
     * @param index The index in the input string from which to start the match.
     * @return The new index in the string if a match was found; the same index otherwise.
     */
    size_t operator()(const std::string& input, size_t index) const;

private:
    /**
     * @brief The matching function to apply.
     *
     */
    match_func_t _func;

    /**
     * @brief A generator for a static-string-matching function.
     *
     * @param static_string The string to match.
     * @return A matching function for the given string.
     */
    static match_func_t _string_func_generator(const std::string& static_string);

    /**
     * @brief A generator for a static-character-matching function.
     *
     * @param static_char The character to match.
     * @return A matching function for the given string.
     */
    static match_func_t _char_func_generator(char static_char);
};


/*
    =======================
    ||  Implementation:  ||
    =======================
*/

match_func::match_func(const match_func::match_func_t& func) : _func(func) {}

match_func::match_func(const std::string& static_string)
    : _func(match_func::_string_func_generator(static_string)) {}

match_func::match_func(char static_char) : _func(match_func::_char_func_generator(static_char)) {}

size_t match_func::operator()(const std::string& input, size_t index) const {
    return this->_func(input, index);
}

match_func::match_func_t match_func::_string_func_generator(const std::string& static_string) {
    return [static_string](const std::string& input_string, size_t index) {
        if (input_string.size() - index < static_string.size()) return index;
        for (size_t ind_offset = 0; ind_offset < static_string.size(); ++ind_offset) {
            if (static_string[ind_offset] != input_string[ind_offset + index]) return index;
        }
        return index + static_string.size();
    };
}

match_func::match_func_t match_func::_char_func_generator(char static_char) {
    return [static_char](const std::string& input_string, size_t index) {
        return index + (input_string[index] == static_char ? 1 : 0);
    };
}

#endif
