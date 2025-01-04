#ifndef EXPR_HPP_CODEGEN
#define EXPR_HPP_CODEGEN

#include "tokenizer.hpp"
#include <vector>
#include "numeric"
#include <format>
#include "hash.hpp"
#include <unordered_set>


namespace Expr{ // namespace


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
    HashType hash;

    Identifier(LiteralType name):
        name(name),
        hash(crc32(name.data(), name.size()))
    {};

    bool operator==(const char* other) const {
        return this->hash == crc32(other);
    }

    bool operator==(const Identifier& rhs) const
    {
        return this->hash == rhs.hash;
    }
};



class IdentifierHasher{
  public:
    std::size_t operator()(const Identifier & id) const
    {
        return id.hash;
    }
};

class IdentifierEqual{
  public:
    bool operator()(const Identifier& lhs, const Identifier& rhs) const
    {
        return lhs == rhs;
    }
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
    Expr(const ExprKind&& kind): kind(std::move(kind)){}

    // template<typename T>
    // explicit Expr(T&& expr) : kind(std::forward<T>(expr)) {}

};

template<typename T>
constexpr bool check_expr_type(const Expr& expr){
    return std::holds_alternative<T>(expr.kind);
}

} // namespace

#endif
