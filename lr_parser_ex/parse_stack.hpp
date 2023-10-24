/**
 * @file parse_stack.hpp
 * @brief Defines the `parse_stack` class.
 * @details Because this is a template class, implementation is in this file.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

#ifndef PARSE_STACK_HPP
#define PARSE_STACK_HPP

//  Header includes:
#include "ast_node.hpp"
#include "production_rule.hpp"

//  STL includes:
#include <functional>
#include <list>
#include <vector>


/*
    ===================
    ||  Definition:  ||
    ===================
*/

/**
 * @brief A stack for `production_item` objects as they are parsed.
 *
 * @tparam token_type_t An enumeration of all token types.
 * @tparam ast_type_t An enumeration of all AST node types.
 */
template<typename token_type_t, typename ast_type_t>
class parse_stack {
public:
    /**
     * @brief The specialized type of an AST node.
     *
     */
    using ast_node_t = ast_node<token_type_t, ast_type_t>;

    /**
     * @brief The specialized type of a production rule.
     *
     */
    using production_rule_t = production_rule<token_type_t, ast_type_t>;

    /**
     * @brief The specialized type of a production item.
     *
     */
    using production_item_t = typename production_rule_t::production_item;

    /**
     * @brief Pushes the given AST node onto the stack.
     *
     * @param node The AST node to push onto the stack.
     */
    void push(const ast_node_t& item);

    /**
     * @brief Applies the given production rule to the top items on the stack.
     *
     * @param rule The production rule to apply.
     */
    void apply_rule(const production_rule_t& rule);

    /**
     * @brief Reports whether the top items on the stack match the given production rule.
     *
     * @param rule The production rule to match.
     * @return True when the top items on the stack match the given production rule.
     *     False otherwise.
     */
    bool matches_rule(const production_rule_t& rule) const;

    /**
     * @brief Reports whether the top items on the stack, plus the next item, make up part of
     *     the given production rule.
     *
     * @param next_item The next item to be pushed onto the stack.
     * @param rule The production rule to check.
     * @return True when the top items on the stack plus the next item match the given
     *     production rule. False otherwise.
     */
    bool matches_rule_part(const production_item_t& next_item, const production_rule_t& rule) const;

    /**
     * @brief Generates AST nodes for all the items in the stack.
     *
     * @tparam node_t The type of an AST node.
     * @param generator The generator function.
     */
    template<typename node_t>
    std::vector<node_t>
    generate_nodes(const std::function<node_t(const ast_node_t&)>& generator) const;

private:
    /**
     * @brief Tests whether the stack matches the given pattern.
     *
     * @param pattern The pattern to test.
     * @return True if the stack matches the given pattern. False otherwise.
     */
    bool _matches_pattern(const std::vector<production_item_t>& pattern) const;

    /**
     * @brief The underlying data structure.
     *
     */
    std::vector<ast_node_t> _items;
};


/*
    =======================
    ||  Implementation:  ||
    =======================
*/

template<typename token_type_t, typename ast_type_t>
void parse_stack<token_type_t, ast_type_t>::push(const ast_node<token_type_t, ast_type_t>& item) {
    _items.emplace_back(item);
}

template<typename token_type_t, typename ast_type_t>
void parse_stack<token_type_t, ast_type_t>::apply_rule(const production_rule_t& rule) {
    const unsigned long size = rule.components.size();
    const std::vector<ast_node<token_type_t, ast_type_t>> child_items {_items.end() - size,
                                                                       _items.end()};
    _items.erase(_items.end() - size, _items.end());
    _items.emplace_back(rule.result.ast_type(), child_items);
}

template<typename token_type_t, typename ast_type_t>
bool parse_stack<token_type_t, ast_type_t>::matches_rule(const production_rule_t& rule) const {
    return _matches_pattern(rule.components);
}

template<typename token_type_t, typename ast_type_t>
bool parse_stack<token_type_t, ast_type_t>::matches_rule_part(const production_item_t& next_item,
                                                              const production_rule_t& rule) const {
    const auto& pattern = rule.components;

    //  Find all instances of the `next_item` in the production rule.
    using index_t = unsigned long;
    std::vector<index_t> next_item_indices;
    for (index_t index = 0; index < pattern.size(); ++index) {
        if (next_item == pattern[index]) next_item_indices.emplace_back(index);
    }

    //  Test whether there is any satisfying partial match.
    for (const index_t index : next_item_indices) {
        if (_matches_pattern({pattern.begin(), pattern.begin() + index})) return true;
    }

    return false;
}

template<typename token_type_t, typename ast_type_t>
template<typename node_t>
std::vector<node_t> parse_stack<token_type_t, ast_type_t>::generate_nodes(
        const std::function<node_t(const ast_node<token_type_t, ast_type_t>&)>& generator) const {
    std::vector<node_t> nodes;

    for (const ast_node<token_type_t, ast_type_t>& item : _items) {
        nodes.emplace_back(generator(item));
    }

    return nodes;
}

template<typename token_type_t, typename ast_type_t>
bool parse_stack<token_type_t, ast_type_t>::_matches_pattern(
        const std::vector<production_item_t>& pattern) const {
    if (pattern.size() > _items.size()) return false;

    for (long pattern_index = pattern.size() - 1; pattern_index >= 0; --pattern_index) {
        const unsigned long items_index = _items.size() - pattern.size() + pattern_index;
        const auto& pattern_item = pattern[pattern_index];
        const auto& stack_item = _items[items_index];
        if ((pattern_item.is_ast_type() != stack_item.is_proper_ast())
            || (pattern_item.is_token_type() != stack_item.is_token())
            || (pattern_item.is_ast_type() && pattern_item.ast_type() != stack_item.ast_type())
            || (pattern_item.is_token_type()
                && pattern_item.token_type() != stack_item.token().type))
            return false;
    }

    return true;
}

#endif
