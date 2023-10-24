/**
 * @file match_pair.hpp
 * @brief Defines the `match_pair` class.
 * @details There are no methods to implement.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef MATCH_PAIR_HPP
#define MATCH_PAIR_HPP

//  Header includes:
#include "match_func.hpp"

//  STL includes:
#include <functional>
#include <string>
#include <utility>


/**
 * @brief Defines a pair of a token type and corresponding matching function.
 *
 * @tparam token_type_t An enumeration of token types.
 */
template<typename token_type_t>
struct match_pair {
    /**
     * @brief The matching function of this pair.
     *
     */
    match_func func;

    /**
     * @brief The token type of this pair.
     *
     */
    token_type_t token_type;
};

#endif  //  MATCH_PAIR_HPP
