#include "ast_parser.hpp"
#include <iostream>
#include "tokenizer.hpp"
#include "visitors/visitors.hpp"
#include "differentiator.hpp"


int main(){
    std::vector<const char*> inputs = {
        // "----+a",
        // "------------++++++++--b",
        // "--+++++++---a",
        // "---a",
        "cos(x)        + (x + 2) + (x - 2)",
        //  "())()()()",
        //  "-EBLOT(EBLO(123,                 aboba, 1232112, AVOVA()      ) ) + 121",
        //  "aboba123 111eblot 566 abjy88yh239f9b8web vacok",
        //  "class aboba ( 1,2,3 )",
        //  "1 * 2 + 3",
        //  "+1 + 223 *345 + 456",
        //  "(123+343)ln(123)",
        //  "123+456",
         // "a+b*123 + a*b + 12321*123 + (1*2 * (a) + b)*a",
         // "a+b*123",
         // "((((((((a))))))))",
         // "a*a*a",
         // "(a+b)*a",
         // "a*b + b*a + 2*a*b",
        "a*b + b*a",
        "a*b",
        "b*a"
        //  "+123 + 123 + 234 + 4534 +12 * (123 * 123 + (1* 2 +4-6))",
        //  "ln(123,log(123*(123*1233434), 434), 432)",
        //  "ln(log(), 123)"
    };

    for (int i = 0; i<inputs.size();i++){
        std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n";
        std::cout << "[INPUT]: '"<< inputs[i] << "'\n";
        Tokenizer::Tokenizer tokenizer;

        auto err = tokenizer.parse_string(inputs[i]);
        if ((size_t)err){
            std::cout << (size_t)err << '\n';
            continue;
        }
        std::cout << "[[ TOKENS START]]\n";
        for (auto& token: tokenizer.tokens){
            std::cout << token << '\n';
        }
        std::cout << "[[ TOKENS END ]]\n\n";

        Parser::Parser parser(tokenizer.tokens);
        try {
            auto expression = parser.expression();
            auto differentiator = Differentiator(expression);
            differentiator.differentiate();
            auto results = differentiator.get_partial_derivatives();

            std::cout << "expr_to_latex: " << Visitors::ExprToLatex(*expression)<< "\n";
            std::cout << "prev tree\n";
            std::cout << Visitors::AstPrinter(*expression) << '\n';
            std::cout << "AFTER DIFF\n**********************************************\n";
            for (const auto& [diff_var, tree] : results){
                std::cout << "\nDifferentiate by variable: " <<  diff_var.name << "\n";
                std::cout << Visitors::AstPrinter(*tree) << '\n';
                std::cout << "AFTER OPTIMIZE:::::\n";
                Visitors::ArithmeticOptimizer(tree);
                // std::cout << Visitors::AstPrinter(*tree) << '\n';
                std::cout << Visitors::ExprToLatex(*tree) << '\n';
                std::cout << "------------\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Exception reason:::: "<< e.what() <<"\n";
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    }
}
