/**
 * @file main.cpp
 * @brief The main LR parser demo.
 * @details This is a brief walkthrough of using the lexer and parser.
 *     Both `lexer.hpp` and `parser.hpp` have more in-depth documentation
 *     on usage, initialization, methods, etc.
 *
 * @copyright Copyright (c) 2023 by Cayden Lund.
 * @license MIT License (https://opensource.org/license/mit).
 *
 */

//  Header includes:
#include "lexer.hpp"
#include "parser.hpp"

//  STL includes:
#include <iostream>

/**
 * @brief An enumeration of all token types.
 *
 */
enum token_type_t {
    INT_TOK,
    PLUS_TOK,
    MINUS_TOK,
    STAR_TOK,
    SLASH_TOK,
    LPAREN_TOK,
    RPAREN_TOK,
    PRINT_TOK
};

/**
 * @brief An enumeration of all AST node types.
 *
 */
enum ast_type_t { EXPR_NODE, PRINT_NODE };

/**
 * @brief Main program entry point.
 *
 */
int main() {
    //  Lexer setup:
    //  ------------

    const auto lex_int = [](const std::string& input, size_t index) -> size_t {
        if (input[index] < '0' || input[index] > '9') return index;

        while ('0' <= input[++index] && input[index] <= '9')
            ;
        return index;
    };

    const std::vector<match_pair<token_type_t>> token_matchers {
            {{lex_int}, INT_TOK}, {{'+'}, PLUS_TOK},   {{'-'}, MINUS_TOK},  {{'*'}, STAR_TOK},
            {{'/'}, SLASH_TOK},   {{'('}, LPAREN_TOK}, {{')'}, RPAREN_TOK}, {{"print"}, PRINT_TOK}};

    lexer<token_type_t> example_lexer(token_matchers);


    //  Parser setup:
    //  -------------

    const std::vector<production_rule<token_type_t, ast_type_t>> production_rules {
            //  `print(<expr>)` --> `<print>`
            {{PRINT_TOK, LPAREN_TOK, EXPR_NODE, RPAREN_TOK}, PRINT_NODE},

            //  `(<expr>)` --> `<expr>`
            {{LPAREN_TOK, EXPR_NODE, RPAREN_TOK}, EXPR_NODE},

            //  `<int>` --> `<expr>`
            {{INT_TOK}, EXPR_NODE},

            //  `<expr> * <expr>` --> `<expr>`
            {{EXPR_NODE, STAR_TOK, EXPR_NODE}, EXPR_NODE},

            //  `<expr> / <expr>` --> `<expr>`
            {{EXPR_NODE, SLASH_TOK, EXPR_NODE}, EXPR_NODE},

            //  `<expr> + <expr>` --> `<expr>`
            {{EXPR_NODE, PLUS_TOK, EXPR_NODE}, EXPR_NODE},

            //  `<expr> - <expr>` --> `<expr>`
            {{EXPR_NODE, MINUS_TOK, EXPR_NODE}, EXPR_NODE}};

    parser<token_type_t, ast_type_t> example_parser(production_rules);


    //  Usage:
    //  ------

    //  The input string.
    //  Try modifying it. See what happens when you feed it invalid tokens,
    //  or a malformed set of tokens (e.g., `3 + + + 4`).
    const std::string input = "print(1 + 22 * 333 + 4444)";

    //  Lex the string into tokens.
    using token_t = token<token_type_t>;
    const std::vector<token<token_type_t>> tokens = example_lexer.lex_string(input);

    //  Parse the tokens into AST nodes.
    using ast_node_t = ast_node<token_type_t, ast_type_t>;
    std::vector<ast_node_t> ast_nodes = example_parser.parse_tokens(tokens);

    //  Various functions for printing the AST nodes.
    const auto token_type_to_str = [](token_type_t type) {
        switch (type) {
            case INT_TOK:
                return "INT_TOK";
            case PLUS_TOK:
                return "PLUS_TOK";
            case MINUS_TOK:
                return "MINUS_TOK";
            case STAR_TOK:
                return "STAR_TOK";
            case SLASH_TOK:
                return "SLASH_TOK";
            case LPAREN_TOK:
                return "LPAREN_TOK";
            case RPAREN_TOK:
                return "RPAREN_TOK";
            case PRINT_TOK:
                return "PRINT_TOK";
        }
    };
    const auto ast_type_to_str = [](ast_type_t type) {
        switch (type) {
            case EXPR_NODE:
                return "EXPR_NODE";
            case PRINT_NODE:
                return "PRINT_NODE";
        }
    };
    const std::function<void(const ast_node_t&, size_t)> print_node = [&](const ast_node_t& node,
                                                                          size_t depth) {
        const std::string indent(4 * depth, ' ');
        if (node.is_proper_ast()) {
            std::cout << indent << "{" << ast_type_to_str(node.ast_type()) << ":" << std::endl;
            for (const auto& child : node.children()) print_node(child, depth + 1);
            std::cout << indent << "}" << std::endl;
        } else {
            const token_t tok = node.token();
            const char delim = tok.text.size() == 1 ? '\'' : '"';
            std::cout << indent << "[" << token_type_to_str(tok.type) << ": " << delim << tok.text
                      << delim << "]" << std::endl;
        }
    };

    //  Print each node.
    for (const auto& node : ast_nodes) print_node(node, 0);

    return 0;
}
