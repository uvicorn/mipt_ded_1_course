#include "tree.hpp"
#include <iostream>
#include "tokenizer.hpp"

int main(){
    char* inputs[20] = {
        "++++++++++-123",
        "cos(x)        + (x + 2) + (x - 2)",
         "())()()()",
         "-EBLOT(EBLO(123,                 aboba, 1232112, AVOVA()      ) ) + 121",
         "aboba123 111eblot 566 abjy88yh239f9b8web vacok",
         "class aboba ( 1,2,3 )"
    };
    for (int i = 0; i<6;i++){
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
            Expr::Expr* expression = parser.expression();
            std::cout << expression->to_string() << '\n';
        }
        catch (const std::exception& e) {
            std::cout << "Exception reason:::: "<< e.what() <<"\n";
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    }
    // ebala(inputs[i]);
}
