#include "ast_parser.hpp"
#include <iostream>
#include "tokenizer.hpp"
#include "visitors/visitors.hpp"

int main(){
    std::vector<const char*> inputs = {
        "----+123",
        // "cos(x)        + (x + 2) + (x - 2)",
        //  "())()()()",
        //  "-EBLOT(EBLO(123,                 aboba, 1232112, AVOVA()      ) ) + 121",
        //  "aboba123 111eblot 566 abjy88yh239f9b8web vacok",
        //  "class aboba ( 1,2,3 )",
        //  "1 * 2 + 3",
        //  "+1 + 223 *345 + 456",
        //  "(123+343)ln(123)",
        //  "123+456",
        //  "a+b",
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
            auto new_expression = Visitors::TreeCopier(expression);
            std::get<Expr::Unary>(std::get<Expr::Unary>(new_expression->kind).right->kind).op = Tokenizer::TokenType::PLUS;

            std::cout << "prev tree\n";
            std::cout << Visitors::AstPrinter(*expression) << '\n';

            std::cout << "new tree\n";
            std::cout << Visitors::AstPrinter(*new_expression) << '\n';
        }
        catch (const std::exception& e) {
            std::cout << "Exception reason:::: "<< e.what() <<"\n";
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    }
}
