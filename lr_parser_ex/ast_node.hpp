/**
 * @file ast_node.hpp
 * @brief Defines the `ast_node` class.
 * @details Because this is a template class, implementation is in this file.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef AST_NODE_HPP
#define AST_NODE_HPP

//  Header includes:
#include "token.hpp"

//  STL includes:
#include <optional>
#include <vector>


/*
    ===================
    ||  Definition:  ||
    ===================
*/

/**
 * @brief Defines an AST node.
 * @details There are two types of AST nodes:
 *     1. An AST node as defined in an `ast_type_t` enumeration.
 *        This is a "proper" AST node.
 *     2. An AST node constructed from a token.
 *        This is done to allow children of AST nodes to be tokens.
 *
 * @tparam token_type_t An enumeration of all token types.
 * @tparam ast_type_t An enumeration of all AST node types.
 */
template<typename token_type_t, typename ast_type_t>
class ast_node {
public:
    /**
     * @brief The specialized type of a token.
     *
     */
    using token_t = token<token_type_t>;

    /**
     * @brief Class constructor.
     * @details Constructs a "proper" AST node.
     *
     * @param ast_type The AST node type of this node.
     * @param children The children of this AST node.
     */
    ast_node(ast_type_t ast_type, const std::vector<ast_node>& children);

    /**
     * @brief Class constructor.
     * @details Implicit constructor from a token.
     *
     * @param token The token that makes up this AST node.
     */
    ast_node(const token_t& token);

    /**
     * @brief Copy constructor.
     *
     * @param other The other AST node to copy.
     */
    ast_node(const ast_node& other);

    /**
     * @brief Whether this AST node is a "proper" AST node.
     * @details I.e., this AST node is not a token.
     *
     * @return True if this AST node is a "proper" AST node; false otherwise.
     */
    [[nodiscard]] bool is_proper_ast() const;

    /**
     * @brief Whether this AST node is a token.
     *
     * @return True if this AST node is a token; false otherwise.
     */
    [[nodiscard]] bool is_token() const;

    /**
     * @brief Gets the AST node type of this node.
     * @details Applicable to "proper" AST nodes only.
     *
     * @return The AST node type of this node.
     */
    [[nodiscard]] ast_type_t ast_type() const;

    /**
     * @brief Gets the token that this node represents.
     * @details Applicable to non-"proper" AST nodes only.
     *
     * @return The token that this node represents.
     */
    [[nodiscard]] token_t token() const;

    /**
     * @brief Gets the AST node children that this node has.
     * @details Applicable to "proper" AST nodes only.
     *
     * @return The children of this node.
     */
    [[nodiscard]] std::vector<ast_node> children() const;

private:
    /**
     * @brief The AST node type of this node.
     * @details Applicable to "proper" AST nodes only.
     *
     */
    std::optional<ast_type_t> _ast_type;

    /**
     * @brief The token that this AST node represents.
     * @details Applicable to non-"proper" AST nodes only.
     *
     */
    std::optional<token_t> _token;

    /**
     * @brief The AST children nodes of this AST node, if any.
     *
     */
    std::optional<std::vector<ast_node>> _children;
};


/*
    =======================
    ||  Implementation:  ||
    =======================
*/

template<typename token_type_t, typename ast_type_t>
ast_node<token_type_t, ast_type_t>::ast_node(ast_type_t ast_type,
                                             const std::vector<ast_node>& children)
    : _ast_type(ast_type), _children(children) {}

template<typename token_type_t, typename ast_type_t>
ast_node<token_type_t, ast_type_t>::ast_node(const token_t& token) : _token(token) {}

template<typename token_type_t, typename ast_type_t>
ast_node<token_type_t, ast_type_t>::ast_node(const ast_node& other)
    : _ast_type(other._ast_type), _token(other._token), _children(other._children) {}

template<typename token_type_t, typename ast_type_t>
bool ast_node<token_type_t, ast_type_t>::is_proper_ast() const {
    return this->_ast_type.has_value();
}

template<typename token_type_t, typename ast_type_t>
bool ast_node<token_type_t, ast_type_t>::is_token() const {
    return this->_token.has_value();
}

template<typename token_type_t, typename ast_type_t>
ast_type_t ast_node<token_type_t, ast_type_t>::ast_type() const {
    return this->_ast_type.value();
}

template<typename token_type_t, typename ast_type_t>
token<token_type_t> ast_node<token_type_t, ast_type_t>::token() const {
    return this->_token.value();
}

template<typename token_type_t, typename ast_type_t>
std::vector<ast_node<token_type_t, ast_type_t>>
ast_node<token_type_t, ast_type_t>::children() const {
    return this->_children.value();
}

#endif
