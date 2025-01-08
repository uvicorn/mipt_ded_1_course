#ifndef DIFF_VISITOR_HPP
#define DIFF_VISITOR_HPP

#include "expr.hpp"
#include <unordered_set>
#include "tokenizer.hpp"
#include "visitors.hpp"
#include "helper.hpp"
#include <iostream>
#include <variant>

#include "expr_operations.hpp"

using Tokenizer::TokenType;

Expr::Expr* CallDifferentiator(const Expr::Call& expr, const Expr::Identifier& diff_id);

// не изменяет исходное дерево, создает новое, не ссылаясь на старое
Expr::Expr* Visitors::Differentiator(const Expr::Expr* root, const Expr::Identifier& diff_id){
    if (root == nullptr)
        return nullptr;

    return std::visit(overloaded{
        [](const Expr::Number& expr) -> Expr::Expr* {
            return NUM(0);
        },

        [diff_id](const Expr::Identifier& expr) -> Expr::Expr* {
            if (diff_id == expr)
                return NUM(1);
            else 
                return NUM(0);
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
            // PLUS, MINUS, DIV, MUL, POW
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
                    auto new_right = MUL(
                        TreeCopier(expr.left),
                        Differentiator(expr.right, diff_id)
                    );
                    auto new_left = MUL(
                        Differentiator(expr.left, diff_id),
                        TreeCopier(expr.right)
                    );
                    return PLUS(new_left, new_right);
                }
                // (u/v)' = (u'v - uv') / v^2
                case TokenType::DIV: {
                    // u'v - uv'
                    auto new_right = MUL(TreeCopier(expr.left), Differentiator(expr.right, diff_id));
                    auto new_left = MUL(Differentiator(expr.left, diff_id), TreeCopier(expr.right));
                    auto numerator = MINUS(new_left, new_right);

                    // 1/v^2
                    auto denominator = POW(TreeCopier(expr.right), NUM(2));
                    // (u/v)' = (u'v - uv') / v^2
                    return DIV(numerator, denominator);
                }
                // (f^g)' = (f^g) * (f*ln(g))' = (f^g) * (f * ln(g))' = (f^g) * (gf'/f + g'ln(f))
                case TokenType::POWER:{
                    auto a = POW(TreeCopier(expr.left), TreeCopier(expr.right));// f^g
                    auto b = DIV(MUL(TreeCopier(expr.right), Differentiator(expr.left, diff_id)), TreeCopier(expr.left)); // gf'/f
                    std::vector<Expr::Expr*> ln_args{ TreeCopier(expr.left)};
                    auto c = MUL(Differentiator(expr.right, diff_id), CALL(ID("ln"), ln_args)); // g'ln(f)
                    return MUL(a, PLUS(b,c));
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
            func_derivative = CALL(ID("cos"), args_copy);
            break;
        }
        case Expr::Identifier("cos"):
            func_derivative = UN_MINUS(CALL(ID("sin"), args_copy));
            break;
        
        case Expr::Identifier("tan"):
            func_derivative = DIV(NUM(1), POW(CALL(ID("cos"), args_copy), NUM(2)));
            break;
        
        case Expr::Identifier("log"):
            func_derivative = DIV(NUM(1), args_copy[0]);
            break;
        default:
            assert(0);
            break;
    }
    auto mul_of_derivatives = MUL(func_derivative, diff_arg);
    return mul_of_derivatives;
}



#endif
