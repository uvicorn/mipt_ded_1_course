#ifndef DIFFERENTIATOR_HPP
#define DIFFERENTIATOR_HPP

#include <utility>
#include "tokenizer.hpp"
#include "visitors/visitors.hpp"
#include <vector>
#include <iostream>
#include "debug.hpp"
#include "ast_parser.hpp"


class Differentiator{
  public:
    struct Output{
        const std::string_view diff_var_name;
        const std::string diff_expr_string;
    };

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

    static std::vector<Output> FromString(const char* input){
        Tokenizer::Tokenizer tokenizer;
        if constexpr(DEBUG)
            std::cout << "[INPUT]: '"<< input << "'\n";

        auto err = tokenizer.parse_string(input);
        if (err != Tokenizer::TokenizerError::OK){
            if constexpr(DEBUG)
                std::cout << (size_t)err << '\n';
            return {};
        }

        if constexpr (DEBUG){
            std::cout << "[[ TOKENS START]]\n";
            for (auto& token: tokenizer.tokens){
                std::cout << token << '\n';
            }
            std::cout << "[[ TOKENS END ]]\n\n";
        }

        Parser::Parser parser(tokenizer.tokens);
        Expr::Expr* expression;
        try {
            expression = parser.expression();
        }
        catch (const std::exception& e) {
            std::cout << "Exception reason:::: "<< e.what() <<"\n";
            return {};
        }

        if constexpr(DEBUG){
            std::cout << "Visitors::ExprToLatex: " << Visitors::ExprToLatex(*expression)<< "\n";
            std::cout << "Visitors::AstPrinter: \n";
            std::cout << Visitors::AstPrinter(*expression) << '\n';
        }

        auto differentiator = Differentiator(expression);
        differentiator.differentiate();
        auto results = differentiator.get_partial_derivatives();

        if constexpr(DEBUG)
            std::cout << "AFTER DIFF\n**********************************************\n";

        std::vector<Output> differentiator_output;

        for (auto& [diff_var, diff_tree] : results){
            if constexpr(DEBUG)
                std::cout << "\nDifferentiate by variable: " <<  diff_var.name << "\n";

            if constexpr(DEBUG){
                std::cout << ":::::BEFORE OPTIMIZE:::::\n";
                std::cout << Visitors::AstPrinter(*diff_tree) << '\n';
                std::cout << ":::::AFTER OPTIMIZE:::::\n";
            }

            diff_tree = Visitors::ArithmeticOptimizer(diff_tree);

            if constexpr(DEBUG){
                std::cout << Visitors::AstPrinter(*diff_tree) << '\n';
                std::cout << "------------\n";
            }

            std::string diff_string = Visitors::ExprToLatex(*diff_tree);
            Visitors::TreeDestroyer(diff_tree);
            differentiator_output.emplace_back(diff_var.name, diff_string);
        }
        Visitors::TreeDestroyer(expression);
        return differentiator_output;
    }
};

#endif
