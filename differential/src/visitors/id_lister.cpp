#include "visitors.hpp"
#include "expr.hpp"

using namespace Visitors;

void IdentifierLister::search_identifiers(const Expr::Expr* root) noexcept{
    if (check_expr_type<Expr::Identifier>(*root)){
        identifiers.insert(std::get<Expr::Identifier>(root->kind));
    }
    else if(check_expr_type<Expr::Unary>(*root)){
        const Expr::Unary& unary = std::get<Expr::Unary>(root->kind);
        search_identifiers(unary.right);
    }
    else if(check_expr_type<Expr::Binary>(*root)){
        const Expr::Binary& binary = std::get<Expr::Binary>(root->kind);

        search_identifiers(binary.right);
        search_identifiers(binary.left);
    }
    else if(check_expr_type<Expr::Grouping>(*root)){
        const Expr::Grouping& group = std::get<Expr::Grouping>(root->kind);
        search_identifiers(group.expr);
    }
    else if(check_expr_type<Expr::Call>(*root)){
        const Expr::Call& call = std::get<Expr::Call>(root->kind);

        // search_identifiers(call.callee);
        for (auto arg: call.args)
            search_identifiers(arg);
    }
}

IdList IdentifierLister::get_identifiers(const Expr::Expr* root) noexcept{
    search_identifiers(root);
    return identifiers;
}

