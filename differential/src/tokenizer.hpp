#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string_view>
#include <vector>
#include <cassert>
#include <map>
#include <iostream>

//
//
namespace Tokenizer {
//
//


enum class TokenizerError {
    OK = 0,
    INVALID_BRACKETS_COUNT,
    INVALID_BRACKETS_ORDER,
};

enum class TokenType {
    // literal
    IDENTIFIER, NUMBER,

    // Single-character tokens
    PLUS, MINUS, DIV, MUL, COMMA,
    RIGHT_PAREN, LEFT_PAREN,
};


using NumValue = long long;
using LiteralType = std::string_view;

struct Token {
    // fields
    TokenType type;
    union{
        NumValue value = 0;
        LiteralType literal;
    };

    // methods

    Token(NumValue value): value(value){
        type = TokenType::NUMBER;
    }
    Token(LiteralType literal): literal(literal){
        type = TokenType::IDENTIFIER;
    }
    Token(TokenType type): type(type){}

};


using TokenList = std::vector<Token>;

class Tokenizer {
  private:

    // 'x10aboba10221' -> {'x10aboba10221'}
    // '10aboba123'    -> {10, 'aboba123'}
    // if argument start with number -> {NUM, ID}, if start with letter -> {ID}
    void parse_literal(const char* literal, size_t literal_length){
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

    void push_literal(char*& literal_start, size_t literal_length){
        if (literal_start){
            parse_literal(literal_start, literal_length);
            literal_start = nullptr;
        }
    }

  public:
    TokenList tokens;

    TokenizerError parse_string(char* str){
        int brackets = 0;
        char* literal_start = nullptr;

        while (*str){
            char symbol = *str;
            switch(symbol){
                case ' ':
                    push_literal(literal_start, str - literal_start);
                    break;

                case '(':
                    push_literal(literal_start, str - literal_start);
                    tokens.emplace_back(TokenType::LEFT_PAREN);
                    brackets++;
                    break;
                case ')':
                    push_literal(literal_start, str - literal_start);
                    tokens.emplace_back(TokenType::RIGHT_PAREN);

                    brackets--;
                    if (brackets < 0)
                        return TokenizerError::INVALID_BRACKETS_COUNT;
                    break;

                case '+':
                    tokens.emplace_back(TokenType::PLUS);
                    break;
                case '-':
                    tokens.emplace_back(TokenType::MINUS);
                    break;
                case '/':
                    tokens.emplace_back(TokenType::DIV);
                    break;
                case '*':
                    tokens.emplace_back(TokenType::MUL);
                    break;

                case ',':
                    push_literal(literal_start, str - literal_start);
                    tokens.emplace_back(TokenType::COMMA);
                    break;
                default:
                    if (literal_start == nullptr)
                        literal_start = str;
            }
            str++;
        }

        push_literal(literal_start, str - literal_start);
        if (brackets != 0)
            return TokenizerError::INVALID_BRACKETS_COUNT;
        return TokenizerError::OK;
    }
};


};

std::ostream& operator<<(std::ostream& out, const Tokenizer::Token token);
std::ostream& operator<<(std::ostream& out, const Tokenizer::TokenType value);

#endif
