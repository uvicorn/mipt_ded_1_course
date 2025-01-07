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


expression     -> term;
term           -> factor ( ( "-" | "+" ) factor )* ;
factor         -> power (( "/" | "*" ) power )* ;
power          -> unary ( "^" unary )* ;
unary          -> ("-"|"+") unary | call ;
call           -> primary ( "(" arguments? ")" )*
arguments      -> expression ( "," expression )*
primary        -> NUMBER | IDENTIFIER | "(" expression ")" ;


expression: term
term:
    | term '+' factor
    | term '-' factor
    | factor

factor:
    | factor '*' unary
    | factor '/' unary
    | unary

unary:
    | '+' unary
    | '-' unary
    | power

power:
    | primary '^' unary
    | call

call: primary ( "(" arguments? ")" )*
arguments: expression ( "," expression )*

primary:
    | NUMBER
    | IDENTIFIER
    | "(" expression ")"

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

    inline bool match(TokenType type){
        if (is_end()){
            return false;
        }
        if (type == current().type){
            current_t_index++;
            return true;
        }

        return false;
    }
    //
    // GRAMMAR IMPLEMENTATION SECTION
    //

    // expression -> term;
    ExprPtr expression(){
        return term();
    }

    // term -> factor ( ( "-" | "+" ) factor )* ;
    ExprPtr term() {
        ExprPtr expr = factor();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            Token op = previous();
            ExprPtr right = factor();
            expr = new Expr::Expr(Expr::Binary(expr, op, right));
        }
        return expr;
    }

    // factor -> power (( "/" | "*" ) power )* ;
    ExprPtr factor(){
        ExprPtr expr = unary();

        while (match({TokenType::MUL, TokenType::DIV})) {
            Token op = previous();
            ExprPtr right = unary();
            expr = new Expr::Expr(Expr::Binary(expr, op, right));
        }
        return expr;
    }

    // power -> unary ( "^" unary )* ;
    // ExprPtr power() {
    //     ExprPtr expr = unary();

    //     while (match(TokenType::POWER)) {
    //         Token op = previous();
    //         ExprPtr right = unary();
    //         expr = new Expr::Expr(Expr::Binary(expr, op, right));
    //     }
    //     return expr;
    // }

    // unary -> ("-"|"+") unary | call ;
    ExprPtr unary(){
        if (match({TokenType::MINUS, TokenType::PLUS})){
            Token operator_token = previous();
            return new Expr::Expr(Expr::Unary(operator_token, unary()));
        }
        return call();
    }

    //call  -> primary ( "(" arguments? ")" )*
    ExprPtr call(){
        ExprPtr expr = primary();

        // loop for case `func()(123)(544)`
        while (true){
            if (match(TokenType::LEFT_PAREN)){
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
            } while(match(TokenType::COMMA));
        }
        Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
        // TODO:
        return new Expr::Expr(Expr::Call(callee, args));
    }

    // primary -> NUMBER | IDENTIFIER | "(" expression ")" ;
    ExprPtr primary(){
        if (match(TokenType::NUMBER)){
            return new Expr::Expr(Expr::Number(previous().value));
        }
        if (match(TokenType::IDENTIFIER)){
            return new Expr::Expr(Expr::Identifier(previous().literal));
        }
        if (match(TokenType::LEFT_PAREN)){
            ExprPtr expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");

            return new Expr::Expr(Expr::Grouping(expr));
        }

        assert(0 && !"No such token in grammar rules");
    }
};



};

#endif
