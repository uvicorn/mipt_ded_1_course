#ifndef DIFF_TREE_HPP
#define DIFF_TREE_HPP

#include <iostream>
#include <memory.h>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <initializer_list>
#include <variant>
#include <string>
#include <string_view>
#include <variant>

#include "expr.hpp"
#include "tokenizer.hpp"
// enum class NodeType: uint8_t {
//     CONST = 0,
//     VAR   = 1,
//     FUNC  = 2,
// };

// enum class FuncName: uint8_t{
//     LN,

//     ADD,
//     SUB,
//     MUL,
//     DIV,
//     UNARY_MINUS,
// };


// class Node;
// using NodePtr = Node*;

// class Node{
//   public:
//     NodeType type;
//     std::size_t childs_count;
//     NodePtr* childs;
//     FuncName func_name;

//     Node() = default;
//     ~Node() = default;

//     Node(FuncName func_name, Node* node1, Node* node2) :
//         func_name(func_name),
//         type(NodeType::FUNC)
//     {
//         childs = new NodePtr[2];
//         childs[0] = node1;
//         childs[1] = node2;
//     }

//     Node(FuncName func_name, size_t childs_count) :
//         func_name(func_name),
//         childs_count(childs_count),
//         type(NodeType::FUNC)
//     {
//         childs = new NodePtr[childs_count];
//     }

//     Node(FuncName func_name, std::initializer_list<Node*> childs) :
//         func_name(func_name),
//         childs_count(childs.size()),
//         type(NodeType::FUNC)
//     {
//         childs = std::move(childs);
//     }

//     Node* operator[](size_t index){
//         return childs[index];
//     }

//     // Node& operator[](size_t index){
//     //     return *childs[index];
//     // }
//     // Node operator=(Node node) = default;
//     Node* copy(){
//         Node* new_node = new Node();
//         std::copy_n(childs, this->childs_count, this->childs);
//     }
    
//     // Node* my_node_copy = my_node.copy_tree()
//     Node* copy_tree(){
//         Node* new_node = new Node(func_name, childs_count);
//         for (size_t index = 0; index < childs_count; index++){
//             if (childs[index] != nullptr)
//                 new_node->childs[index] = childs[index]->copy_tree();
//             else
//                 new_node->childs[index] = nullptr;
//         }
//         return new_node;
//     }
//     // ~Node(){
//     //     delete childs;
//     // }
// };



/*
https://craftinginterpreters.com/parsing-expressions.html#recursive-descent-parsing


expression     -> term ;
term           -> factor ( ( "-" | "+" ) factor )* ;
factor         -> unary (( "/" | "*" ) unary )* ;
unary          -> ("-"|"+") unary | call ;
call           -> primary ( "(" arguments? ")" )*
arguments      -> expression ( "," expression )*
primary        -> NUMBER | IDENTIFIER | "(" expression ")" ;
*/

namespace Parser {

using namespace Tokenizer;
// using namespace Expr;

class Parser{
  private:
    const TokenList tokens;
    size_t current_t_index = 0;
  public:
    Parser(TokenList& tokens):
        tokens(std::move(tokens))
    {}

    inline Token previous() const{
        return tokens[current_t_index - 1];
    }

    inline Token current() const{
        return tokens[current_t_index];
    }
    inline Token current(){
        return tokens[current_t_index];
    }

    inline Token advance(){
        if (is_end())
            return tokens[current_t_index];
        else
            return tokens[current_t_index++];
    }

    Token consume(TokenType type, const char* message){
        if (check_tokentype(type))
            return advance();

        throw std::runtime_error(message);
    }

    inline bool is_end() const{
        return current_t_index  == tokens.size();
    }

    inline bool check_tokentype(TokenType type) const{
        return !is_end() && type == current().type;
    }

    inline bool match(std::initializer_list<TokenType> types){
        assert(types.size()>0);

        if (is_end()){
            std::cout << "ISEND\n";
            return false;
        }

        for (auto type: types){
            if (type == current().type){
                current_t_index++;
                return true;
            }
        }

        return false;
    }

    //
    // GRAMMAR IMPLEMENTATION SECTION
    //

    // expression -> term;
    Expr::Expr expression(){
        std::cout << "EXPRESSION\n";
        return term();
    }

    // term -> factor ( ( "-" | "+" ) factor )* ;
    Expr::Expr term() {
        std::cout << "TERM START\n";
        Expr::Expr expr = factor();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            std::cout << "TERM MINUS PLUS\n";
            Token op = previous();
            Expr::Expr right = factor();
            expr = Expr::Binary(expr, op, right);
        }
        return expr;
    }

    // factor -> unary (( "/" | "*" ) unary )* ;
    Expr::Expr factor(){
        std::cout << "FACTOR START\n";
        Expr::Expr expr = unary();

        while (match({TokenType::MUL, TokenType::DIV})) {
            std::cout << "FACTOR MUL DIV\n";
            Token op = previous();
            Expr::Expr right = unary();
            expr = Expr::Binary(expr, op, right);
        }
        return expr;
    }

    // unary -> ("-"|"+") unary | call ;
    Expr::Expr unary(){
        std::cout << "UNARY START\n";
        // std::cout << current().type << '\n';
        if (match({TokenType::MINUS, TokenType::PLUS})){
            TokenType operator_type = previous().type;
            std::cout << "UNARY MINUS\n";
            return Expr::Unary(operator_type, unary());
        }
        return call();
    }

    //call  -> primary ( "(" arguments? ")" )*
    Expr::Expr call(){
        std::cout << "CALL START\n";
        Expr::Expr expr = primary();

        // loop for case `func()(123)(544)`
        while (true){
            if (match({TokenType::LEFT_PAREN})){
                // std::cout << 
                expr = get_func_args(expr);
            } else {
                break;
            }
        }
        return expr;
    }

    // arguments -> expression ( "," expression )*
    Expr::Expr get_func_args(Expr::Expr callee){
        std::vector<Expr::Expr> args;
        if (!check_tokentype(TokenType::RIGHT_PAREN)){
            do {
                args.push_back(expression());
            } while(match({TokenType::COMMA}));
        }
        Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
        // TODO:
        return Expr::Call(callee, paren, args);
    }

    // primary -> NUMBER | IDENTIFIER | "(" expression ")" ;
    Expr::Expr primary(){
        std::cout << "CURRENT" << current() << '\n';
        std::cout << "PRIMARY START\n";
        if (match({TokenType::NUMBER})){
            std::cout << "PRIMARY NUMBER\n";
            return Expr::Number(previous().value);
        }
        if (match({TokenType::IDENTIFIER})){
            std::cout << "PRIMARY ID\n";
            return Expr::Identifier(previous().literal);
        }
        if (match({TokenType::LEFT_PAREN})){
            std::cout << "PRIMARY LEFT_PAREN\n";
            Expr::Expr expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");

            return Expr::Grouping(expr);
        }
        assert(0 && !"EBALAAALSALA");
    }
};




};

#endif
