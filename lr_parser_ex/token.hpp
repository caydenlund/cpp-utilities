/**
 * @file token.hpp
 * @brief Defines the `token` class.
 * @details Because this is a template class, implementation is in this file.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef TOKEN_HPP
#define TOKEN_HPP

//  STL includes:
#include <string>


/*
    ===================
    ||  Definition:  ||
    ===================
*/

/**
 * @brief Represents a lexed token.
 *
 * @tparam token_type_t An enumeration of all token types.
 */
template<typename token_type_t>
class token {
public:
    /**
     * @brief Default constructor.
     *
     */
    token() = default;

    /**
     * @brief Explicit constructor.
     *
     * @param type The type of this token.
     * @param line The line on which this token starts.
     * @param column The column on which this token starts.
     * @param text The text contained within this token.
     */
    token(token_type_t type, size_t line, size_t column, std::string text);

    /**
     * @brief The type of this token.
     *
     */
    token_type_t type;

    /**
     * @brief The text contained within this token.
     *
     */
    std::string text;

    /**
     * @brief The line on which this token starts.
     *
     */
    size_t line {};

    /**
     * @brief The column on which this token starts.
     *
     */
    size_t column {};
};


/*
    =======================
    ||  Implementation:  ||
    =======================
*/

template<typename token_type_t>
token<token_type_t>::token(token_type_t type, size_t line, size_t column, std::string text)
    : type(type), text(std::move(text)), line(line), column(column) {}

#endif
