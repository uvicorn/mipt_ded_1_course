#ifndef EXPR_HPP_CODEGEN
#define EXPR_HPP_CODEGEN

#include "tokenizer.hpp"
#include <vector>
#include "numeric"
#include <format>


namespace Expr{

using namespace Tokenizer;


struct Expr;


struct Call {
    Expr* callee;
    Token right_paren;
    std::vector<Expr*> args;

    Call(Expr* callee, Token right_paren, std::vector<Expr*>& args):
        callee(callee),
        right_paren(right_paren),
        args(std::move(args))
    {
    };

};


struct Identifier {
    LiteralType name;

    Identifier(LiteralType name):
        name(name)
    {
    };

};

struct Number{
    NumValue value;

    Number(NumValue value):
        value(value)
    {
    };

};

struct Binary {
    Expr* left;
    Token op;
    Expr* right;

    Binary(Expr* left, Token op, Expr* right):
        left(left),
        op(op),
        right(right)
    {
    };

};

struct Unary {
    Token op;
    Expr* right;

    Unary(Token op, Expr* right):
        op(op),
        right(right)
    {
    };

};

struct Grouping {
    Expr* expr;

    Grouping(Expr* expr):
        expr(expr)
    {
    };

};

using ExprKind = std::variant<
    Number,
    Identifier,
    Binary,
    Unary,
    Call,
    Grouping
>;

struct Expr {
    // SourceSpan span;
    ExprKind kind;
    Expr(const ExprKind&& kind): kind(kind){}
    // Expr(const Expr& expr){
        
    // }
    // // operator
};

//
//
};
//
//

#endif

