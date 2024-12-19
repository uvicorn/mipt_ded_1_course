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
    virtual std::string to_string() const noexcept = 0;
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
    T accept(Visitor<T>& visitor){
        return visitor.visitCall(this);
    }

    std::string to_string() const noexcept{
        std::string args_str = "";
        for (auto arg: args)
            args_str += arg->to_string();
        // std::string args_str = std::accumulate(
        //     args.begin(), 
        //     args.end(), 
        //     std::string(), 
        //     [](Expr* a, Expr* b) {
        //         return a->to_string() + ", " + b->to_string();
        //     }
        // );

        return std::format("Call(func={}, args=[{}])", callee->to_string(), args_str);
    }
};


class Identifier : public Expr {
  public:
    LiteralType name;

    Identifier(LiteralType name):
        name(name)
    {};

    template<typename T>
    T accept(Visitor<T>& visitor){
        return visitor.visitIdentifier(this);
    }

    std::string to_string() const noexcept{
        return std::format("Identifier(name='{}')", name);
    }
};

class Number : public Expr {
  public:
    NumValue value;

    Number(NumValue value):
        value(value)
    {};

    template<typename T>
    T accept(Visitor<T>& visitor){
        return visitor.visitNumber(this);
    }
    std::string to_string() const noexcept{
        return std::format("Number(value={})", value);
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
    T accept(Visitor<T>& visitor){
        return visitor.visitBinary(this);
    }

    std::string to_string() const noexcept{
        return std::format("Binary(left={}, right={}, Op={})", left->to_string(), right->to_string(), ::to_string(op.type));
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
    T accept(Visitor<T>& visitor){
        return visitor.visitUnary(this);
    }
    std::string to_string() const noexcept{
        return std::format("Unary(right={}, Op={})", right->to_string(), ::to_string(op.type));
    }
};

class Grouping : public Expr {
  public:
    Expr* expr;

    Grouping(Expr* expr):
        expr(expr)
    {};

    template<typename T>
    T accept(Visitor<T>& visitor){
        return visitor.visitGrouping(this);
    }
    std::string to_string() const noexcept{
        return std::format("Grouping(expr={})", expr->to_string());
    }
};


};
#endif
