#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string_view>
#include <vector>


enum class TokenizerError {
    OK = 0,
    INVALID_BRACKETS_COUNT,
    INVALID_BRACKETS_ORDER,
};

enum class TokenType {
    Operator,

    RightParen,
    LeftParen,

    Literal,
    Const
};

using ConstValue = long long;

struct Token {
    TokenType type;

    union{
    std::string_view literal = {};
    ConstValue value;
    };
};

using TokenList = std::vector<Token>;


class Tokenizer {
  private:
    void push_literal(char*& literal_start, size_t literal_length){
        if (literal_start){
            tokens.push_back(Token{
                .type = TokenType::Literal,
                .literal = {literal_start, literal_length}
            });
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
            if (symbol == '('){

                push_literal(literal_start, str - literal_start);
                tokens.push_back(Token{.type = TokenType::RightParen});

                brackets++;

            } else if (symbol == ')'){
                push_literal(literal_start, str - literal_start);
                tokens.push_back(Token{.type = TokenType::LeftParen});

                brackets--;
                if (brackets < 0)
                    return TokenizerError::INVALID_BRACKETS_COUNT;

            } else if (symbol == '+' || symbol == '/' || symbol == '*' || symbol == '-'){
                tokens.push_back(Token{
                    .type = TokenType::Operator,
                    .literal = {str, 1}
                });

            } else if (symbol == ' ' || symbol == ','){
                push_literal(literal_start, str - literal_start);
            } else if (literal_start == nullptr){
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


#endif
