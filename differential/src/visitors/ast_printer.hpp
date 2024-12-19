#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include "expr.hpp"

//
//
namespace Visitors{
//
//

class AstPrinter: public Expr::Visitor<std::string>{
  public:
    std::string print(Expr::Expr* expr){
        return expr->accept(this);
    }

    std::string visitNumber(Expr::Number* expr){
        return std::format("Number(value={})", expr->value);
    }

    std::string visitIdentifier(Expr::Identifier* expr){
        return std::format("Identifier(name='{}')", expr->name);
    }

    std::string visitBinary(Expr::Binary* expr){
        return std::format(
            "Binary(op={}, left={}, right={})",
            to_string(expr->op.type), expr->left->accept(this), expr->right->accept(this)
        );
    }

    std::string visitUnary(Expr::Unary* expr){
        return std::format(
            "Unary(op={}, right={})",
            to_string(expr->op.type), expr->accept(this)
        );
    }

    std::string visitCall(Expr::Call* expr){
        std::string args_str = "";
        for (auto arg: expr->args)
            args_str += arg->accept(this);
        // std::string args_str = std::accumulate(
        //     args.begin(), 
        //     args.end(), 
        //     std::string(), 
        //     [](Expr* a, Expr* b) {
        //         return a->to_string() + ", " + b->to_string();
        //     }
        // );

        return std::format("Call(func={}, args=[{}])", expr->callee->accept(this), args_str);
    }

    std::string visitGrouping(Expr::Grouping* expr){
        return std::format("Grouping(expr={})", expr->accept(this));
    }
};

//
//
}
//
//


#endif
