#include "expr.hpp"
#include "visitors.hpp"
#include <optional>
#include "helper.hpp"

// TODO: протестить perf на версии с queue
// https://www.geeksforgeeks.org/non-recursive-program-to-delete-an-entire-binary-tree/
[[nodiscard]] std::nullptr_t Visitors::TreeDestroyer(Expr::Expr* root){
    if (root == nullptr)
        return nullptr;

    std::visit(overloaded{
        [](const Expr::Number& expr) {},
        [](const Expr::Identifier& expr) {},

        [](const Expr::Binary& expr) {
            TreeDestroyer(expr.left);
            TreeDestroyer(expr.right);
        },

        [](const Expr::Unary& expr) {
            TreeDestroyer(expr.right);
        },

        [](const Expr::Call& expr){
            TreeDestroyer(expr.callee);
            for (auto arg: expr.args)
                TreeDestroyer(arg);
        },

        [](const Expr::Grouping& expr){
            TreeDestroyer(expr.expr);
        }
    }, root->kind);

    delete root;
    return nullptr;
}
