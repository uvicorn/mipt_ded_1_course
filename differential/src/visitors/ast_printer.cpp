#include <string>
#include <variant>
#include "expr.hpp"
#include "helper.hpp"
#include "visitors.hpp"
#include <memory>
#include <iostream>
#include <iomanip>
#include <vector>


//
// https://lesleylai.info/en/ast-in-cpp-part-1-variant/
//

std::string to_hex_string(const void *ptr) {
    std::stringstream ss;
    ss << "0x" << std::hex << std::setfill('0') <<  std::left << reinterpret_cast<uintptr_t>(ptr);
    return ss.str();
}


namespace Visitors{ // namespace

// К сожалению я не смог написать хороший паттерн Visitor на плюсах. Поэтому это вот это

[[nodiscard]] std::string AstPrinter(const Expr::Expr& expr, size_t indent_size){
    auto result = std::visit(overloaded{
        [](const Expr::Number& expr){
            return std::format("Number(value={})", expr.value);
        },

        [](const Expr::Identifier& expr){
            return std::format("Identifier(name='{}')", expr.name);
        },

        [indent_size](const Expr::Binary& expr){
            std::string indent(indent_size * 4, ' ');

            return std::format(
                "Binary(\n{0}    op={1},\n{0}    left={2},\n{0}    right={3}\n{0})",
                indent,
                to_string(expr.op.type),
                AstPrinter(*expr.left, indent_size+1),
                AstPrinter(*expr.right, indent_size+1)
            );
        },

        [indent_size](const Expr::Unary& expr){
            std::string indent(indent_size * 4, ' ');

            return std::format(
                "Unary(\n{0}    op={1},\n{0}    right={2}\n{0})",
                indent,
                to_string(expr.op.type),
                AstPrinter(*expr.right, indent_size+1)
            );
        },

        [indent_size](const Expr::Call& expr){
            std::string indent(indent_size * 4, ' ');

            std::string args_str = "";
            if (expr.args.size() != 0){
                for (auto arg: expr.args){
                    args_str += std::format(
                        "{}        {},\n",
                        indent, AstPrinter(*arg, indent_size+2)
                    );
                }
                args_str = std::format("\n{1}    {0}", indent, args_str);
            }

            return std::format("Call(\n{0}    func={1},\n{0}    args=[{2}]\n{0})",
                indent,
                AstPrinter(*expr.callee, indent_size+1),
                args_str
            );
        },

        [indent_size](const Expr::Grouping& expr){
            std::string indent(indent_size * 4, ' ');

            return std::format("Grouping(\n{0}    expr={1}\n{0})",
                indent,
                AstPrinter(*expr.expr, indent_size+1)
            );
        },
    }, expr.kind);

    return result;
    // std::string indent(indent_size * 4, ' ');
    // return result +"\n"+ indent + "    " + "ADDRESS=" + to_hex_string(&expr)+ ")";
}

} // namespace
