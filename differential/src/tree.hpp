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
using ExprPtr = Expr::Expr*;

class Parser{
  private:
    const TokenList tokens;
    size_t current_t_index = 0;
  public:

    Parser(TokenList& tokens): tokens(std::move(tokens)){}

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
    ExprPtr expression(){
        std::cout << "EXPRESSION\n";
        return term();
    }

    // term -> factor ( ( "-" | "+" ) factor )* ;
    ExprPtr term() {
        std::cout << "TERM START\n";
        ExprPtr expr = factor();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            std::cout << "TERM MINUS PLUS\n";
            Token op = previous();
            ExprPtr right = factor();
            expr = new Expr::Binary(expr, op, right);
        }
        return expr;
    }

    // factor -> unary (( "/" | "*" ) unary )* ;
    ExprPtr factor(){
        std::cout << "FACTOR START\n";
        ExprPtr expr = unary();

        while (match({TokenType::MUL, TokenType::DIV})) {
            std::cout << "FACTOR MUL DIV\n";
            Token op = previous();
            ExprPtr right = unary();
            expr = new Expr::Binary(expr, op, right);
        }
        return expr;
    }

    // unary -> ("-"|"+") unary | call ;
    ExprPtr unary(){
        std::cout << "UNARY START\n";
        // std::cout << current().type << '\n';
        if (match({TokenType::MINUS, TokenType::PLUS})){
            TokenType operator_type = previous().type;
            std::cout << "UNARY MINUS\n";
            return new Expr::Unary(operator_type, unary());
        }
        return call();
    }

    //call  -> primary ( "(" arguments? ")" )*
    ExprPtr call(){
        std::cout << "CALL START\n";
        ExprPtr expr = primary();

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
    ExprPtr get_func_args(ExprPtr callee){
        std::vector<ExprPtr> args;
        if (!check_tokentype(TokenType::RIGHT_PAREN)){
            do {
                args.push_back(expression());
            } while(match({TokenType::COMMA}));
        }
        Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
        // TODO:
        return new Expr::Call(callee, paren, args);
    }

    // primary -> NUMBER | IDENTIFIER | "(" expression ")" ;
    ExprPtr primary(){
        std::cout << "CURRENT" << current() << '\n';
        std::cout << "PRIMARY START\n";
        if (match({TokenType::NUMBER})){
            std::cout << "PRIMARY NUMBER\n";
            return new Expr::Number(previous().value);
        }
        if (match({TokenType::IDENTIFIER})){
            std::cout << "PRIMARY ID\n";
            return new Expr::Identifier(previous().literal);
        }
        if (match({TokenType::LEFT_PAREN})){
            std::cout << "PRIMARY LEFT_PAREN\n";
            ExprPtr expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");

            return new Expr::Grouping(expr);
        }

        assert(0 && !"EBALAAALSALA");
    }
};




};

#endif
