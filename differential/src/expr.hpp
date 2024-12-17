#ifndef EXPR_HPP_CODEGEN
#define EXPR_HPP_CODEGEN

#include "tokenizer.hpp"
#include <vector>

namespace Expr{

using namespace Tokenizer;


class Expr{};


class Call : public Expr{
  public:
    Expr callee;
    Token right_paren;
    std::vector<Expr> args;
    Call(Expr callee, Token right_paren, std::vector<Expr>& args):
        callee(callee),
        right_paren(right_paren),
        args(std::move(args))
    {};
};
class Identifier : public Expr{
  public:
    LiteralType name;
    Identifier(LiteralType name):
        name(name)
    {};
};

class Number : public Expr{
  public:
    NumValue value;
    Number(NumValue value):
        value(value)
    {};
};

class Binary : public Expr{
  public:
    Expr left;
    Token op;
    Expr right;
    Binary(Expr left, Token op, Expr right):
        left(left),
        op(op),
        right(right)
    {};
};

class Unary : public Expr{
  public:
    Token op;
    Expr right;
    Unary(Token op, Expr right):
        op(op),
        right(right)
    {};
};

class Grouping : public Expr{
  public:
    Expr expr;
    Grouping(Expr expr):
        expr(expr)
    {};
};

};
#endif