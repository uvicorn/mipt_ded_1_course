#ifndef DIFF_VISITOR_HPP
#define DIFF_VISITOR_HPP

#include "expr.hpp"
#include <unordered_set>
#include "visitors.hpp"
#include "expr.hpp"

namespace Visitors{


class Differentiator{
  private:
    IdList identifiers;

  public:
    Expr::Expr* new_tree = nullptr;

    Differentiator(const Expr::Expr* root){

    }

    void differentiate(const Expr::Expr* root, const Expr::Identifier& id){
        if (check_expr_type<Expr::Identifier>(*root)){
            identifiers.insert(std::get<Expr::Identifier>(root->kind));
        }
    }

    void visit_binary(const Expr::Expr& root){
        auto binary = std::get<Expr::Binary>(root.kind);
    }
    void visit_identifier(cosnt)
};


}

#endif
