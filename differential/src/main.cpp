#include "tree.hpp"
#include <iostream>
#include "tokenizer.hpp"
#include "visitors/visitors.hpp"

int main(){
    std::vector<const char*> inputs = {
        "++++++++++-123",
        "cos(x)        + (x + 2) + (x - 2)",
         "())()()()",
         "-EBLOT(EBLO(123,                 aboba, 1232112, AVOVA()      ) ) + 121",
         "aboba123 111eblot 566 abjy88yh239f9b8web vacok",
         "class aboba ( 1,2,3 )",
         "1 * 2 + 3",
         "+1 + 223 *345 + 456"
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
            auto result = Visitors::AstPrinter(*expression);

            std::cout << result << '\n';
        }
        catch (const std::exception& e) {
            std::cout << "Exception reason:::: "<< e.what() <<"\n";
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    }
    // ebala(inputs[i]);
}
