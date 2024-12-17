#include "tokenizer.hpp"

using namespace Tokenizer;

std::ostream& operator<<(std::ostream& out, const TokenType type){
    #define INSERT_ELEMENT(p) \
        case p:               \
            return out << #p; \
            break;

    switch(type){
        INSERT_ELEMENT(TokenType::IDENTIFIER);
        INSERT_ELEMENT(TokenType::NUMBER);
        INSERT_ELEMENT(TokenType::MINUS);
        INSERT_ELEMENT(TokenType::DIV);
        INSERT_ELEMENT(TokenType::PLUS);
        INSERT_ELEMENT(TokenType::MUL);
        INSERT_ELEMENT(TokenType::COMMA);
        INSERT_ELEMENT(TokenType::RIGHT_PAREN);
        INSERT_ELEMENT(TokenType::LEFT_PAREN);
    }
    #undef INSERT_ELEMENT
}

std::ostream& operator<<(std::ostream& out, const Token token){
    switch(token.type){
        case TokenType::IDENTIFIER:
            return out << "IDENTIFIER '"<< token.literal  <<"'";
            break;
        case TokenType::NUMBER:
            return out << "NUMBER "<< token.value;
            break;
        default:
            return out << token.type;
    }
}


