#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include <string>
#include "visitors.hpp"


//
//
namespace Visitors{
//
//

class AstPrinter: public Expr::Visitor<std::string>{
    std::string visitNumber(Expr::Number* expr){
        return std::to_string(expr->value);
    }
    std::string visitIdentifier(Expr::Identifier* expr){
        return expr->name;
    }
    std::string visitBinary(Expr::Binary* expr){
        return "(" + expr->left + expr->op->type + expr->right +")";
    }
    std::string visitUnary(Expr::Unary* expr){
        return "(" + expr->op + expr->right +")";
    }

    std::string visitGrouping(Expr::Grouping* expr){
        return "(" + expr->expr + ")";
    }

};

//
//
}
//
//
#endif
