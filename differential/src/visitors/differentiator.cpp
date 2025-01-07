#ifndef DIFF_VISITOR_HPP
#define DIFF_VISITOR_HPP

#include "expr.hpp"
#include <unordered_set>
#include "tokenizer.hpp"
#include "visitors.hpp"
#include "helper.hpp"
#include <iostream>
#include <variant>

using Tokenizer::TokenType;

Expr::Expr* CallDifferentiator(const Expr::Call& expr, const Expr::Identifier& diff_id);

// не изменяет исходное дерево, создает новое, не ссылаясь на старое
Expr::Expr* Visitors::Differentiator(const Expr::Expr* root, const Expr::Identifier& diff_id){
    if (root == nullptr)
        return nullptr;

    return std::visit(overloaded{
        [](const Expr::Number& expr) -> Expr::Expr* {
            return new Expr::Expr(Expr::Number(0));
        },

        [diff_id](const Expr::Identifier& expr) -> Expr::Expr* {
            if (diff_id == expr)
                return new Expr::Expr(Expr::Number(1));
            else 
                return new Expr::Expr(Expr::Number(0));
        },

        [diff_id](const Expr::Unary& expr) -> Expr::Expr* {
            return new Expr::Expr(
                Expr::Unary(expr.op, Differentiator(expr.right, diff_id))
            );
        },

        [diff_id](const Expr::Grouping& expr) -> Expr::Expr* {
            return new Expr::Expr(
                Expr::Grouping(Differentiator(expr.expr, diff_id))
            );
        },

        [diff_id](const Expr::Binary& expr) -> Expr::Expr* {
            // PLUS, MINUS, DIV, MUL

            switch (expr.op.type){
                // (u+v)' = u' + v'
                // (u-v)' = u' - v'
                case TokenType::PLUS:
                case TokenType::MINUS:
                    return new Expr::Expr(
                        Expr::Binary(Differentiator(expr.left, diff_id), expr.op.type, Differentiator(expr.right, diff_id))
                    );

                // (u*v)' = u'v + uv'
                case TokenType::MUL: {
                    auto new_right = new Expr::Expr(Expr::Binary(
                        TreeCopier(expr.left),
                        TokenType::MUL,
                        Differentiator(expr.right, diff_id)
                    ));
                    auto new_left = new Expr::Expr(Expr::Binary(
                        Differentiator(expr.left, diff_id),
                        TokenType::MUL,
                        TreeCopier(expr.right)
                    ));
                    return new Expr::Expr(
                        Expr::Binary(new_left, TokenType::PLUS, new_right)
                    );
                }
                // (u/v)' = (u'v - uv') / v^2
                case TokenType::DIV: {
                    // u'v - uv'
                    auto new_right = new Expr::Expr(
                        Expr::Binary(TreeCopier(expr.left), TokenType::MUL, Differentiator(expr.right, diff_id))
                    );
                    auto new_left = new Expr::Expr(
                        Expr::Binary(Differentiator(expr.left, diff_id), TokenType::MUL, TreeCopier(expr.right))
                    );
                    auto numerator = new Expr::Expr(
                        Expr::Binary(new_left, TokenType::MINUS, new_right)
                    );
                    // 1/v^2
                    auto right_cp = TreeCopier(expr.right);
                    auto denominator = new Expr::Expr(
                        Expr::Binary(right_cp, TokenType::MUL, right_cp)
                    );
                    // (u/v)' = (u'v - uv') / v^2
                    return new Expr::Expr(
                        Expr::Binary(numerator, TokenType::DIV, denominator)
                    );
                }
                default:
                    assert(0);
                    return nullptr;
            }
        },

        [diff_id](const Expr::Call& expr) -> Expr::Expr* {
            return CallDifferentiator(expr, diff_id);
        },
    }, root->kind);
}

// (f(u(x)))' = f'(u(x))u'(x)
Expr::Expr* CallDifferentiator(const Expr::Call& expr, const Expr::Identifier& diff_id){
    assert(expr.args.size() == 1 && "Only one argument functions are supported now!");
    assert(Expr::check_expr_type<Expr::Identifier>(*expr.callee));

    auto func_id = std::get<Expr::Identifier>(expr.callee->kind);
    auto args_copy = Visitors::CallArgsCopier(expr.args);
    auto diff_arg = Visitors::Differentiator(expr.args[0], diff_id);
    Expr::Expr* func_derivative = nullptr;

    switch(func_id){
        case Expr::Identifier("sin"):{
            auto diff_func = new Expr::Expr(Expr::Identifier("cos"));
            func_derivative = new Expr::Expr(Expr::Call(diff_func, args_copy));
            break;
        }
        case Expr::Identifier("cos"):{
            auto diff_func = new Expr::Expr(Expr::Identifier("sin"));
            func_derivative = new Expr::Expr(
                Expr::Unary(
                    TokenType::MINUS,
                    new Expr::Expr(Expr::Call(diff_func, args_copy))
                )
            );
            break;
        }
        case Expr::Identifier("tan"):{
            auto diff_func = new Expr::Expr(Expr::Identifier("cos"));
            auto cos2 = new Expr::Expr(Expr::Call(diff_func, args_copy));
            func_derivative = new Expr::Expr(
                Expr::Binary(
                    new Expr::Expr(Expr::Number(1)),
                    TokenType::DIV,
                    new Expr::Expr(Expr::Binary(cos2, TokenType::MUL, cos2))
                )
            );
            break;
        }
        case Expr::Identifier("log"):
            func_derivative = new Expr::Expr(
                Expr::Binary(
                    new Expr::Expr(Expr::Number(1)),
                    TokenType::DIV,
                    args_copy[0]
                )
            );
            break;
        default:
            assert(0);
            break;
    }
    auto mul_of_derivatives = new Expr::Expr(Expr::Binary(func_derivative, Tokenizer::TokenType::MUL, diff_arg));
    return mul_of_derivatives;
}



#endif
