#ifndef DIFF_VISITOR_HPP
#define DIFF_VISITOR_HPP

#include "expr.hpp"
#include <unordered_set>
#include "visitors.hpp"
#include "helper.hpp"
#include <iostream>


namespace Visitors{

Expr::Expr* Differentiator(const Expr::Expr* root, const Expr::Identifier& diff_id){
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
            using Tokenizer::TokenType;

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

        // TODO
        [diff_id](const Expr::Call& expr) -> Expr::Expr* {
            return new Expr::Expr(expr);
        },
    }, root->kind);
}



}

#endif
