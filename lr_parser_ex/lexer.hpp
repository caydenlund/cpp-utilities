/**
 * @file lexer.cpp
 * @brief Defines the `lexer` class.
 * @details Because this is a template class, implementation is in this file.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef LEXER_HPP
#define LEXER_HPP

//  Header includes:
#include "match_pair.hpp"
#include "token.hpp"

//  STL includes:
#include <functional>
#include <initializer_list>
#include <sstream>
#include <string>
#include <vector>


/*
    ===================
    ||  Definition:  ||
    ===================
*/

/**
 * @brief Lexes a string into tokens.
 * @details
 *
 *     First, an enumeration of all token types is provided as a template parameter.
 *
 *         ```
 *         enum token_type_t {
 *             INT_TOK,     //  `<int>`
 *             PLUS_TOK,    //  `+`
 *             MINUS_TOK,   //  `-`
 *             STAR_TOK,    //  `*`
 *             SLASH_TOK,   //  `/`
 *             LPAREN_TOK,  //  `(`
 *             RPAREN_TOK,  //  `)`
 *             PRINT_TOK    //  `print`
 *         };
 *
 *         using lexer_t = lexer<token_type_t>;
 *         ```
 *
 *     To construct a `lexer` instance, provide a mapping of token types to corresponding
 *     token-matching functions.
 *     A token-matching function accepts two arguments: the string to match, and the index into
 *     the string from which to start the match.
 *     Upon successfully matching its token type, the function returns the new index from which
 *     to lex. Upon failure, the function returns the same index.
 *
 *     These functions, then, have the following signature:
 *
 *         ```
 *         using matching_func_t = std::function<size_t(const std::string&, size_t)>;
 *         ```
 *
 *     Token-matching functions can also be default-constructed from a string or character.
 *
 *     Make these mappings by providing a list of pairs (matching function, resulting token type).
 *
 *         ```
 *         const matching_func_t lex_int = [](const std::string& input, size_t index) -> size_t {
 *             if ('0' > input[index] || input[index] > '9') return index;
 *
 *             while ('0' < input[++index] && input[index] < '9')
 *                 ;
 *
 *             return index - 1;
 *         };
 *
 *         const std::vector<match_pair<token_type_t>> token_matchers {
 *             {{lex_int}, INT_TOK},   //  Use this explicit matching function.
 *             {{'+'}, PLUS_TOK},      //  Implicit construction from a character.
 *             {{'-'}, MINUS_TOK},     //  Implicit construction from a character.
 *             {{'*'}, STAR_TOK},      //  Implicit construction from a character.
 *             {{'/'}, SLASH_TOK},     //  Implicit construction from a character.
 *             {{'('}, LPAREN_TOK},    //  Implicit construction from a character.
 *             {{')'}, RPAREN_TOK},    //  Implicit construction from a character.
 *             {{"print"}, PRINT_TOK}  //  Implicit construction from a string.
 *         };
 *         ```
 *
 *     Then, instantiate the lexer with the list of token-matching rules.
 *
 *         ```
 *         lexer_t example_lexer(token_matchers);
 *         ```
 *
 *     After construction, use the lexer to lex an input string.
 *     Upon success, the lexer returns a series of `token`s.
 *     Upon failure, an exception is thrown.
 *
 *         ```
 *         using token_t = token<token_type_t>;
 *         const std::vector<token_t> tokens = lexer_inst.lex_string("print(1 + 2 * 3 + 4)");
 *
 *         //  Throws an exception:
 *         //  `lexer_inst.lex_string("XYZ");`
 *         ```
 *
 *     The lexer ignores spaces by default.
 *     To change this behavior, pass the parameter `false` to the `lex_string` method:
 *
 *         ```
 *         //  Throws an exception:
 *         //  `lexer_inst.lex_string("print(1 + 2 * 3 + 4)", false);`
 *         ```
 *
 * @tparam token_type_t An enumeration of all token types.
 */
template<typename token_type_t>
class lexer {
public:
    /**
     * @brief The specialized type of a token.
     *
     */
    using token_t = token<token_type_t>;

    /**
     * @brief The specialized type of a matching pair.
     */
    using match_pair_t = match_pair<token_type_t>;

    /**
     * @brief Class constructor.
     *
     * @param match_pairs The mapping of token types to their corresponding token-matching
     *     functions.
     */
    lexer(const std::vector<match_pair_t>& match_pairs);

    /**
     * @brief Lexes the given string.
     *
     * @param input The string to lex.
     * @param ignore_spaces Whether to ignore spaces. Defaults to true.
     * @return An ordered list of `token` objects.
     */
    std::vector<token_t> lex_string(const std::string& input, bool ignore_spaces = true) const;

private:
    /**
     * @brief The set of token types and corresponding token-matching functions.
     *
     */
    const std::vector<match_pair<token_type_t>> _match_pairs;
};


/*
    =======================
    ||  Implementation:  ||
    =======================
*/

template<typename token_type_t>
lexer<token_type_t>::lexer(const std::vector<match_pair<token_type_t>>& match_pairs)
    : _match_pairs(match_pairs) {}

template<typename token_type_t>
std::vector<token<token_type_t>> lexer<token_type_t>::lex_string(const std::string& input,
                                                                 bool ignore_spaces) const {
    //  The final return vector.
    std::vector<token<token_type_t>> tokens;

    //  Line and column trackers, for use in instantiating token objects.
    //  These are used for printing error messages.
    size_t line_num = 1;
    size_t column_num = 1;
    std::stringstream line;

    //  Ignore leading spaces.
    size_t index = 0;
    while (ignore_spaces && input[index] == ' ') ++index;

    while (index < input.size()) {
        bool found_match = false;
        for (const auto& matching_pair : _match_pairs) {
            size_t new_index = matching_pair.func(input, index);

            //  If this matching function is successful, do the following:
            if (new_index > index) {
                found_match = true;
                const std::string text = input.substr(index, new_index - index);

                //  Add the new token to the list.
                tokens.emplace_back(matching_pair.token_type, line_num, column_num, text);

                //  Ignore trailing spaces.
                while (ignore_spaces && input[new_index] == ' ') ++new_index;

                //  Update line/column variables.
                for (size_t sub_ind = index; sub_ind < new_index; ++sub_ind) {
                    if (input[sub_ind] == '\n') {
                        ++line_num;
                        column_num = 1;
                        line.str("");
                    } else {
                        ++column_num;
                        line << input[sub_ind];
                    }
                }

                index = new_index;
                break;
            }
        }

        //  If we've tried every lexing function,
        //  and none of them matched:
        //  TODO: ???
        if (!found_match) {
            constexpr size_t max_substr_len = 20;
            const size_t input_len = input.size() - index;
            const size_t substr_len = (max_substr_len < input_len) ? max_substr_len : input_len;
            std::string substr = input.substr(index, substr_len);
            for (size_t sub_ind = 0; sub_ind < substr_len; ++sub_ind) {
                if (substr[sub_ind] == '\n') {
                    substr = substr.substr(0, sub_ind);
                    break;
                }
            }

            std::stringstream error_message;
            error_message << "Invalid token at " << line_num << ":" << column_num << ":"
                          << std::endl
                          << "    " << line.str() << substr << std::endl
                          << std::string(line.str().size() + 4, ' ')
                          << std::string(substr.size(), '~') << std::endl;
            throw std::runtime_error(error_message.str());
        }
    }

    return tokens;
}

#endif
