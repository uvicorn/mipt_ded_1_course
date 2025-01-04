#include "expr.hpp"
#include "visitors.hpp"
#include <utility>
#include <variant>
#include "helper.hpp"




Expr::Expr* Visitors::TreeCopier(const Expr::Expr* root){
    if (root == nullptr)
        return nullptr;

    return std::visit(overloaded{
        [](const Expr::Number& expr) -> Expr::Expr* {
            return new Expr::Expr(expr);
        },
        [](const Expr::Identifier& expr) -> Expr::Expr* {
            return new Expr::Expr(expr);
        },

        [](const Expr::Binary& expr) -> Expr::Expr* {
            auto new_left  = TreeCopier(expr.left);
            auto new_right = TreeCopier(expr.right);
            return new Expr::Expr(Expr::Binary(new_left, expr.op, new_right));
        },

        [](const Expr::Unary& expr) -> Expr::Expr* {
            auto new_right = TreeCopier(expr.right);
            return new Expr::Expr(Expr::Unary(expr.op, new_right));
        },

        [](const Expr::Call& expr) -> Expr::Expr* {
            auto new_callee = TreeCopier(expr.callee);
            std::vector<Expr::Expr*> new_args(expr.args.size());
            for (size_t index = 0; index < expr.args.size(); index++)
                new_args[index] = TreeCopier(expr.args[index]);
            return new Expr::Expr(Expr::Call(new_callee, expr.right_paren, new_args));
        },

        [](const Expr::Grouping& expr) -> Expr::Expr* {
            auto new_expr = TreeCopier(expr.expr);
            return new Expr::Expr(Expr::Grouping(new_expr));
        }
    }, root->kind);
}

Expr::Expr* Visitors::NodeCopier(const Expr::Expr& node){
    return new Expr::Expr(node);
}
