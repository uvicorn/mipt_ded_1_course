#ifndef DIFFERENTIATOR_HPP
#define DIFFERENTIATOR_HPP

#include <utility>
#include "visitors/visitors.hpp"
#include <vector>
#include <iostream>


class Differentiator{
  public:
    Expr::Expr* root;
    Visitors::IdList id_list;
    std::vector<std::pair<Expr::Identifier, Expr::Expr*>> result_pairs;

    Differentiator(Expr::Expr* root):
        root(root)
    {
        auto id_lister = Visitors::IdentifierLister();
        id_list = id_lister.get_identifiers(root);
        result_pairs.reserve(id_list.size());
    }

    void differentiate(){
        for(auto diff_var : id_list){
            auto diff_result = Visitors::Differentiator(root, diff_var);
            result_pairs.emplace_back(diff_var, diff_result);
        }
    }
    decltype(result_pairs) get_partial_derivatives() const noexcept{
        return result_pairs;
    }
};

#endif
