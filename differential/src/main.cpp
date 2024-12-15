#include "tree.hpp"
#include <iostream>
#include "tokenizer.hpp"

void ebala(char* input){

    Tokenizer tokenizer;
    TokenizerError parse_error =  tokenizer.parse_string(input);
    std::cout << "PARSE_ERROR " << static_cast<int>(parse_error) << '\n';
    
    std::cout <<"INPUT: '"<< input << "'\n";
    std::cout << "OUTPUT:\n";
    for (Token& token : tokenizer.tokens){
        switch (token.type){
            case TokenType::Operator:
                std::cout << "OPERATOR " << token.literal << "\n";
                break;
            case TokenType::LeftParen:
                std::cout << "CLOSE_BRACKET" "\n";
                break;
            case TokenType::RightParen:
                std::cout << "OPEN_BRACKET" "\n";
                break;
            case TokenType::Literal:
                std::cout << "LITERAL '" << token.literal <<"'\n";
                break;
        }
        // std::cout << static_cast<int>(token.type) << "\n";
        // std::cout << token.literal << "\n";
        // std::cout << "\n";
    }
    std::cout << "\n\n";
}

int main(){
    char* input = "cos(x)        + (x + 2) + (x - 2)";
    char* input2 = "())()()()";
    char* input3 = "-EBLOT(EBLO(123,                 aboba, 1232112, AVOVA()      ) ) + 121";
    ebala(input);
    ebala(input2);
    ebala(input3);
}
