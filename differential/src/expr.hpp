#ifndef EXPR_HPP_CODEGEN
#define EXPR_HPP_CODEGEN

#include "tokenizer.hpp"
#include <vector>
#include "numeric"
#include <format>
#include "hash.hpp"
#include <unordered_set>


namespace Expr{ // namespace

struct Expr;

using namespace Tokenizer;

struct Call {
    Expr* callee;
    // Token right_paren;
    std::vector<Expr*> args;

    Call(Expr* callee, std::vector<Expr*>& args):
        callee(callee),
        // right_paren(right_paren),
        args(std::move(args))
    {
    };
};


struct Identifier {
    LiteralType name;
    HashType hash;

    // compile-time hash
    template<size_t N>
    consteval Identifier(const char (&str)[N]):
        name(str),
        hash(crc32_constexpr(str, N-1))
    {}

    Identifier(LiteralType name):
        name(name),
        hash(crc32(name.data(), name.size()))
    {}


    constexpr operator int() const {
        return hash;
    }

    bool operator==(const char* other) const {
        return this->hash == crc32(other);
    }
    constexpr bool operator==(const Identifier& rhs) const
    {
        return this->hash == rhs.hash;
    }
};



class IdentifierHasher{
  public:
    std::size_t operator()(const Identifier& id) const
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

    constexpr Binary(Expr* left, Token op, Expr* right):
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

    inline Expr* operator+(Expr* b){
        return new Expr(Binary(this, TokenType::PLUS, b));
    }
    inline Expr* operator-(Expr* b){
        return new Expr(Binary(this, TokenType::MINUS, b));
    }
    inline Expr* operator*(Expr* b){
        return new Expr(Binary(this, TokenType::MUL, b));
    }
    inline Expr* operator/(Expr* b){
        return new Expr(Binary(this, TokenType::DIV, b));
    }
    inline Expr* operator^(Expr* b){
        return new Expr(Binary(this, TokenType::POWER, b));
    }
};

template<typename T>
constexpr bool check_expr_type(const Expr& expr){
    return std::holds_alternative<T>(expr.kind);
}

template<typename T>
constexpr const T& get_expr_kind(const Expr& expr){
    return std::get<T>(expr.kind);
}

} // namespace

using ExprPtr = Expr::Expr*;

#endif
