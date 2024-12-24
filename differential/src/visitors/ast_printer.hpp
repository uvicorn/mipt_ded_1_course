#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include <variant>
#include "expr.hpp"
#include "helper.hpp"
// #include 

//
// https://lesleylai.info/en/ast-in-cpp-part-1-variant/
//

//
//
namespace Visitors{
//
//

// К сожалению я не смог написать хороший паттерн Visitor на плюсах. Поэтому это вот это

[[nodiscard]] auto AstPrinter(const Expr::Expr& expr) -> std::string {
    return std::visit(overloaded{
        [](const Expr::Number& expr) {
            return std::format("Number(value={})", expr.value);
        },
        [](const Expr::Identifier& expr) {
            return std::format("Identifier(name='{}')", expr.name);
        },
        [](const Expr::Binary& expr) { 
            return std::format(
                "Binary(op={}, left={}, right={})",
                to_string(expr.op.type), AstPrinter(*expr.left), AstPrinter(*expr.right)
            );
        },
        [](const Expr::Unary& expr) {
            return std::format(
                "Unary(op={}, right={})",
                to_string(expr.op.type), AstPrinter(*expr.right)
            );
        },
        [](const Expr::Call& expr){
            std::string args_str = "";
            for (auto arg: expr.args)
                args_str += AstPrinter(*arg);
            // std::string args_str = std::accumulate(
            //     args.begin(), 
            //     args.end(), 
            //     std::string(), 
            //     [](Expr* a, Expr* b) {
            //         return a->to_string() + ", " + b->to_string();
            //     }
            // );

            return std::format("Call(func={}, args=[{}])", AstPrinter(*expr.callee), args_str);
        },
        [](const Expr::Grouping& expr) {
            return std::format("Grouping(expr={})", AstPrinter(*expr.expr));
        },
    }, expr.kind);
}

//
//
}
//
//


#endif
