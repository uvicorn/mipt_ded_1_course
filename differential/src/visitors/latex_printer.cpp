#include "visitors.hpp"
#include "helper.hpp"

namespace Visitors{

[[nodiscard]] std::string ExprToLatex(const Expr::Expr& expr){
    return std::visit(overloaded{
        [](const Expr::Number& expr){
            return std::format("{}", expr.value);
        },

        [](const Expr::Identifier& expr){
            return std::string(expr.name);
        },

        [](const Expr::Binary& expr){
            return std::format(
                "({0}{1}{2})",
                ExprToLatex(*expr.left),
                to_latex(expr.op.type),
                ExprToLatex(*expr.right)
            );
        },

        [](const Expr::Unary& expr){
            return std::format(
                "{0}{1}",
                to_latex(expr.op.type),
                ExprToLatex(*expr.right)
            );
        },

        [](const Expr::Call& expr){
            std::string args_str = "";
            if (expr.args.size() != 0){
                for (auto arg: expr.args){
                    args_str += std::format(
                        "{}, ",
                        ExprToLatex(*arg)
                    );
                }
            }

            return std::format("{0}({1})",
                ExprToLatex(*expr.callee),
                args_str
            );
        },

        [](const Expr::Grouping& expr){
            return std::format("({0})",
                ExprToLatex(*expr.expr)
            );
        },
    }, expr.kind);
}

}
