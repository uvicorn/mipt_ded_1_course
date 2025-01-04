#include "tokenizer.hpp"
#include <format>


using namespace Tokenizer;


std::string_view to_string(TokenType type){
    #define INSERT_ELEMENT(p,name) \
        case p:               \
            return #name;

    switch(type){
        INSERT_ELEMENT(TokenType::IDENTIFIER, Id);
        INSERT_ELEMENT(TokenType::NUMBER, Number);
        INSERT_ELEMENT(TokenType::MINUS, Minus);
        INSERT_ELEMENT(TokenType::DIV, Div);
        INSERT_ELEMENT(TokenType::PLUS, Plus);
        INSERT_ELEMENT(TokenType::MUL, Mul);
        INSERT_ELEMENT(TokenType::COMMA, Comma);
        INSERT_ELEMENT(TokenType::RIGHT_PAREN, RightParen);
        INSERT_ELEMENT(TokenType::LEFT_PAREN, LeftParen);
    }
    #undef INSERT_ELEMENT
}

std::string_view to_latex(TokenType type){
    #define INSERT_ELEMENT(tokentype, str) \
        case tokentype:                    \
            return str;                    

    switch (type){
        INSERT_ELEMENT(TokenType::MINUS, "-")
        INSERT_ELEMENT(TokenType::PLUS, "+")
        INSERT_ELEMENT(TokenType::MUL, "*")
        INSERT_ELEMENT(TokenType::DIV, "/")
        default: return "???";
    }
    #undef INSERT_ELEMENT
}




std::ostream& operator<<(std::ostream& out, const TokenType type){
    return out << to_string(type);
}

std::string to_string(Token token){
    switch(token.type){
        case TokenType::IDENTIFIER:
            return std::format("Token(type=Id, value={})", token.literal);
            break;
        case TokenType::NUMBER:
            return std::format("Token(type=Number, value={})", token.value);
            break;
        default:
            return std::format("Token(type={})", to_string(token.type));
    }
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

namespace Tokenizer { // namespace


// 'x10aboba10221' -> {'x10aboba10221'}
// '10aboba123'    -> {10, 'aboba123'}
// if argument start with number -> {NUM, ID}, if start with letter -> {ID}
void Tokenizer::push_literal(const char* literal, size_t literal_length){
    assert(literal_length);


    NumValue num_value = 0; // FEATURE: переписать на бигинт
    bool is_numeric = '0' <= literal[0] && literal[0] <= '9';
    size_t index = 0;

    while (index < literal_length){
        char symbol = literal[index];
        if ('0' <= symbol && symbol <= '9'){
            num_value = 10*num_value + (symbol - '0');
        } else {
            break;
        }
        index++;
    }

    if (is_numeric){
        tokens.emplace_back(num_value);
    }

    if (literal_length - index){
        if (is_numeric)
            tokens.emplace_back(TokenType::MUL);
        tokens.emplace_back( LiteralType{literal + index, literal_length - index});
    }
}

void Tokenizer::checkpush_literal(const char* literal_start, size_t literal_length){
    if (to_push_literal){
        push_literal(literal_start, literal_length);
        to_push_literal = false;
    }
}


TokenizerError Tokenizer::parse_string(const char* str){
    int brackets = 0;
    const char* literal_start = nullptr;

    while (*str){
        char symbol = *str;
        switch(symbol){
            case ' ':
                checkpush_literal(literal_start, str - literal_start);
                break;

            case '(':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::LEFT_PAREN);
                brackets++;
                break;

            case ')':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::RIGHT_PAREN);

                brackets--;
                if (brackets < 0)
                    return TokenizerError::INVALID_BRACKETS_COUNT;
                break;

            case '+':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::PLUS);
                break;
            case '-':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::MINUS);
                break;
            case '/':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::DIV);
                break;
            case '*':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::MUL);
                break;

            case ',':
                checkpush_literal(literal_start, str - literal_start);
                tokens.emplace_back(TokenType::COMMA);
                break;

            default:
                if (!to_push_literal){
                    literal_start = str;
                    to_push_literal = true;
                }
        }
        str++;
    }

    checkpush_literal(literal_start, str - literal_start);
    if (brackets != 0)
        return TokenizerError::INVALID_BRACKETS_COUNT;
    return TokenizerError::OK;
}


};// namespace
