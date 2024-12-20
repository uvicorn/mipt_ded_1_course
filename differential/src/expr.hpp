#ifndef EXPR_HPP_CODEGEN
#define EXPR_HPP_CODEGEN

#include "tokenizer.hpp"
#include <vector>
#include "numeric"
#include <format>


namespace Expr{

using namespace Tokenizer;


// без этой еблени visitor ничерта не видит
class Expr;
class Number;
class Identifier;
class Binary;
class Grouping;
class Unary;
class Call;


//
// СПАСИБО АРСЕНИЮ
//
template<typename Visitor>
concept Visitable = requires(Visitor visitor) {
    { visitor.visitNumber(std::declval<Number*>())         };
    { visitor.visitIdentifier(std::declval<Identifier*>()) };
    { visitor.visitUnary(std::declval<Unary*>())           };
    { visitor.visitBinary(std::declval<Binary*>())         };
    { visitor.visitCall(std::declval<Call*>())             };
    { visitor.visitGrouping(std::declval<Grouping*>())     };
};

template<typename T>
class Visitor{
  public:
    T visitNumber(Number* expr);
    T visitIdentifier(Identifier* expr);
    T visitUnary(Unary* expr);
    T visitBinary(Binary* expr);
    T visitCall(Call* expr);
    T visitGrouping(Grouping* expr);

    // static_assert(Visitable<Visitor<T>>,
    //     "Visitor must implement all required visit methods");
};



class Expr{
  public:
    template<typename T>
    T accept(Visitor<T>* visitor);
};


class Call : public Expr {
  public:
    Expr* callee;
    Token right_paren;
    std::vector<Expr*> args;

    Call(Expr* callee, Token right_paren, std::vector<Expr*>& args):
        callee(callee),
        right_paren(right_paren),
        args(std::move(args))
    {};

    template<typename T>
    T accept(Visitor<T>* visitor){
        return visitor->visitCall(this);
    }
};


class Identifier : public Expr {
  public:
    LiteralType name;

    Identifier(LiteralType name):
        name(name)
    {};

    template<typename T>
    T accept(Visitor<T>* visitor){
        return visitor->visitIdentifier(this);
    }

};

class Number : public Expr {
  public:
    NumValue value;

    Number(NumValue value):
        value(value)
    {};

    template<typename T>
    T accept(Visitor<T>* visitor){
        return visitor->visitNumber(this);
    }
};

class Binary : public Expr {
  public:
    Expr* left;
    Token op;
    Expr* right;

    Binary(Expr* left, Token op, Expr* right):
        left(left),
        op(op),
        right(right)
    {};

    template<typename T>
    T accept(Visitor<T>* visitor){
        return visitor->visitBinary(this);
    }

};

class Unary : public Expr {
  public:
    Token op;
    Expr* right;

    Unary(Token op, Expr* right):
        op(op),
        right(right)
    {};

    template<typename T>
    T accept(Visitor<T>* visitor){
        return visitor->visitUnary(this);
    }
};

class Grouping : public Expr {
  public:
    Expr* expr;

    Grouping(Expr* expr):
        expr(expr)
    {};

    template<typename T>
    T accept(Visitor<T>* visitor){
        return visitor->visitGrouping(this);
    }
};


};
#endif
