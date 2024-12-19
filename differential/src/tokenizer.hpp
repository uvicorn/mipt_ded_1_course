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
    // Literals
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
    union{ // TODO убрать union и хранить и число, и литерал
        NumValue value = 0;
        LiteralType literal;
    };


    Token(NumValue value):      type(TokenType::NUMBER),     value(value){}
    Token(LiteralType literal): type(TokenType::IDENTIFIER), literal(literal){}
    Token(TokenType type):      type(type){}
};


using TokenList = std::vector<Token>;

class Tokenizer {
  private:
    void parse_literal(const char* literal, size_t literal_length);
    void push_literal(const char*& literal_start, size_t literal_length);
  public:
    TokenList tokens;
    TokenizerError parse_string(const char* str);
};


//
//
};
//
//


std::ostream& operator<<(std::ostream& out, const Tokenizer::Token token);
std::ostream& operator<<(std::ostream& out, const Tokenizer::TokenType value);

std::string to_string(Tokenizer::Token     token);
std::string to_string_op(Tokenizer::TokenType type);
std::string to_string(Tokenizer::TokenType type);


#endif
