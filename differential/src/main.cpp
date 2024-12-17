#include "tree.hpp"
#include <iostream>
#include "tokenizer.hpp"

// void ebala(char* input){

//     Tokenizer tokenizer;
//     TokenizerError parse_error =  tokenizer.parse_string(input);
//     std::cout << "PARSE_ERROR " << static_cast<int>(parse_error) << '\n';

//     std::cout <<"INPUT: '"<< input << "'\n";
//     std::cout << "OUTPUT:\n";
//     for (Token& token : tokenizer.tokens){
//         switch (token.type){
//             case TokenType::MUL:
//                 std::cout << "*" << "\n";
//                 break;
//             case TokenType::DIV:
//                 std::cout << "/" << "\n";
//                 break;
//             case TokenType::MINUS:
//                 std::cout << "-" << "\n";
//                 break;
//             case TokenType::PLUS:
//                 std::cout << "+" << "\n";
//                 break;
//             case TokenType::IDENTIFIER:
//                 std::cout << "ID "<< token.literal <<"\n";
//                 break;
//             case TokenType::RIGHT_PAREN:
//                 std::cout << ")" <<"\n";
//                 break;
//             case TokenType::LEFT_PAREN:
//                 std::cout << "("<< "\n";
//                 break;
//             case TokenType::NUMBER:
//                 std::cout << "NUM " << token.value << "\n";
//                 break;
//         }
//         // std::cout << static_cast<int>(token.type) << "\n";
//         // std::cout << token.literal << "\n";
//         // std::cout << "\n";
//     }
//     std::cout << "\n\n";
// }

// using namespace Tokenizer;
// using namespace Parser;

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
            Expr::Expr expression = parser.expression();
        }
        catch (const std::exception& e) {
            std::cout << "Exception reason:::: "<< e.what() <<"\n";
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    }
    // ebala(inputs[i]);
}
