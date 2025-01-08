#ifndef EXPR_HELPER_HPP
#define EXPR_HELPER_HPP

#include "expr.hpp"

#define MUL(a,b) (new Expr::Expr(Expr::Binary(a, Tokenizer::TokenType::MUL, b)))
#define DIV(a,b) (new Expr::Expr(Expr::Binary(a, Tokenizer::TokenType::DIV, b)))
#define POW(a,b) (new Expr::Expr(Expr::Binary(a, Tokenizer::TokenType::POWER, b)))
#define PLUS(a,b) (new Expr::Expr(Expr::Binary(a, Tokenizer::TokenType::PLUS, b)))

#define MINUS(a,b) (new Expr::Expr(Expr::Binary(a, Tokenizer::TokenType::MINUS, b)))
#define UN_MINUS(a) (new Expr::Expr(Expr::Unary(Tokenizer::TokenType::MINUS, a)))

#define NUM(a) (new Expr::Expr(Expr::Number(a)))
#define ID(a) (new Expr::Expr(Expr::Identifier(a)))
#define CALL(a, args) (new Expr::Expr(Expr::Call(a, args)))

#endif
