/**
 * @file production_rule.hpp
 * @brief Defines the `production_rule` class.
 * @details Because this is a template class, implementation is in this file.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef PRODUCTION_RULE_HPP
#define PRODUCTION_RULE_HPP

//  STL includes:
#include <optional>
#include <vector>


/*
    ===================
    ||  Definition:  ||
    ===================
*/

/**
 * @brief Represents a production rule for the parser.
 * @details A production rule is made up of two parts: the result,
 *     and the components that are combined to create the result.
 *
 *     For example, you might want to make a production rule for addition expressions.
 *     In this case, you would want two sub-expressions, along with the '+' token.
 *     You might construct this production, rule, then, as the following:
 *
 *         ```
 *         //  Define types.
 *         enum token_type_t {INT_TOK, PLUS_TOK};
 *         enum ast_node_type_t {EXPR_NODE};
 *         using production_rule_t = production_rule<token_t, ast_node_t>;
 *
 *         //  integer --> expression.
 *         const production_rule_t int_expr_rule(EXPR_NODE, {INT_TOK});
 *
 *         //  expression + expression --> expression.
 *         const production_rule_t plus_expr_rule(EXPR_NODE, {EXPR_NODE, PLUS_TOK, EXPR_NODE});
 *         ```
 *
 * @tparam token_type_t An enumeration of all token types.
 * @tparam ast_type_t An enumeration of all AST node types.
 */
template<typename token_type_t, typename ast_type_t>
class production_rule {
public:
    /**
     * @brief Defines an item in a production rule.
     * @details Is made up of either a token or an AST node.
     *
     */
    class production_item {
    public:
        /**
         * @brief Class constructor.
         * @details Implicit constructor from an AST node type.
         *
         * @param ast_type The AST node type that makes up this production item.
         */
        production_item(ast_type_t ast_type);

        /**
         * @brief Class constructor.
         * @details Implicit constructor from a token type.
         *
         * @param token_type The token type that makes up this production item.
         */
        production_item(token_type_t token_type);

        /**
         * @brief Equality operator.
         *
         * @param other The other production item against which to compare.
         * @return True when the other production item is equal to this one. False otherwise.
         */
        bool operator==(const production_item& other) const;

        /**
         * @brief Inequality operator.
         *
         * @param other The other production item against which to compare.
         * @return False when the other production item is equal to this one. True otherwise.
         */
        bool operator!=(const production_item& other) const;

        /**
         * @brief Reports whether this production item represents an AST node type.
         *
         * @return True if this production item represents an AST node type; false otherwise.
         */
        [[nodiscard]] bool is_ast_type() const;

        /**
         * @brief Reports whether this production item represents a token type.
         *
         * @return True if this production item represents a token type; false otherwise.
         */
        [[nodiscard]] bool is_token_type() const;

        /**
         * @brief Reports the AST node type that this production item represents.
         *
         * @return The AST node type that this production item represents.
         */
        [[nodiscard]] ast_type_t ast_type() const;

        /**
         * @brief Reports the token type that this production item represents.
         *
         * @return The token type that this production item represents.
         */
        [[nodiscard]] token_type_t token_type() const;

    private:
        /**
         * @brief The AST node type that this production item represents, if any.
         *
         */
        const std::optional<ast_type_t> _ast_type;

        /**
         * @brief The token type that this production item represents, if any.
         *
         */
        const std::optional<token_type_t> _token_type;
    };

    /**
     * @brief Class constructor.
     *
     * @param components The (token or AST node) items that, when combined, create the result.
     * @param result The (AST node) result of the production rule.
     */
    production_rule(const std::vector<production_item>& components, const production_item& result);

    /**
     * @brief The result of applying this production rule.
     *
     */
    const production_item result;

    /**
     * @brief The components that, when combined, create the result.
     */
    const std::vector<production_item> components;
};


/*
    =======================
    ||  Implementation:  ||
    =======================
*/

template<typename token_type_t, typename ast_type_t>
production_rule<token_type_t, ast_type_t>::production_rule(
        const std::vector<production_rule::production_item>& components,
        const production_rule::production_item& result)
    : result(result), components(components) {}

template<typename token_type_t, typename ast_type_t>
production_rule<token_type_t, ast_type_t>::production_item::production_item(ast_type_t ast_type)
    : _ast_type(ast_type) {}

template<typename token_type_t, typename ast_type_t>
production_rule<token_type_t, ast_type_t>::production_item::production_item(token_type_t token_type)
    : _token_type(token_type) {}

template<typename token_type_t, typename ast_type_t>
bool production_rule<token_type_t, ast_type_t>::production_item::operator==(
        const production_rule::production_item& other) const {
    if (this->is_token_type()) {
        if (!other.is_token_type()) return false;

        return this->token_type() == other.token_type();
    }

    if (this->is_ast_type()) {
        if (!other.is_ast_type()) return false;

        return this->ast_type() == other.ast_type();
    }

    return false;
}

template<typename token_type_t, typename ast_type_t>
bool production_rule<token_type_t, ast_type_t>::production_item::operator!=(
        const production_rule::production_item& other) const {
    return !(*this == other);
}

template<typename token_type_t, typename ast_type_t>
bool production_rule<token_type_t, ast_type_t>::production_item::is_ast_type() const {
    return this->_ast_type.has_value();
}

template<typename token_type_t, typename ast_type_t>
bool production_rule<token_type_t, ast_type_t>::production_item::is_token_type() const {
    return this->_token_type.has_value();
}

template<typename token_type_t, typename ast_type_t>
ast_type_t production_rule<token_type_t, ast_type_t>::production_item::ast_type() const {
    return this->_ast_type.value();
}

template<typename token_type_t, typename ast_type_t>
token_type_t production_rule<token_type_t, ast_type_t>::production_item::token_type() const {
    return this->_token_type.value();
}

#endif
