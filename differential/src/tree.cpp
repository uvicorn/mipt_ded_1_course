#include "tree.hpp"
#include <cctype>
#include <string>
#include "latex.hpp"
#include <vector>

#include <string>
#include <string_view>
#include <variant>


/*
https://craftinginterpreters.com/parsing-expressions.html#recursive-descent-parsing


expression     → equality ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary
               | primary ;
primary        → NUMBER | STRING | "true" | "false" | "nil"
               | "(" expression ")" ;
*/


// Node* FromString(ParseInfo* info, Node* root){
//     std::vector<std::string_view> args = {};
//     // long long arg = 0;
//     char* arg_start = NULL;
//     // std::string_view numeric_arg;
//     // bool is_number = false;
//     char op = NULL;

//     while (info->str[0]){
//         char symbol = info->str[0];

//         if (symbol == '('){
//             info->brackets_count++;
//             info->str++;

//             if (arg_start != NULL){
//                 FromString(info, root);
//             }
//         } else if (symbol == ' ' || symbol == ','){
//             std::string_view arg{arg_start, static_cast<size_t>(info->str - arg_start)};
//             args.push_back(arg);
//             arg_start = NULL;
//             info->str++;
//             continue;
//         } else if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/'){
//             op = symbol;
//             info->str++;
//             FromString(info.str, root);
//         } else if(arg_start == NULL){
//             arg_start = info->str;
//         }
//         if (symbol == ')'){
//             info->brackets_count--;
//         }
//         info->str++;
//     }
//     info->str++;

//     return root;
// }
// };

class Parser{
    
};

std::string ToString(Node* root){
    if (root == nullptr)
        return "";

    #define BIND_HANDLER(OP_NAME, HANDLER)      \
        case (OP_NAME):                         \
            return HANDLER(root);               \

    switch (root->func_name){
        BIND_HANDLER(FuncName::LN,  LatexParser::ln);
        BIND_HANDLER(FuncName::ADD, LatexParser::add);
        BIND_HANDLER(FuncName::SUB, LatexParser::sub);
        BIND_HANDLER(FuncName::DIV, LatexParser::div);
        BIND_HANDLER(FuncName::MUL, LatexParser::mul);
        BIND_HANDLER(FuncName::UNARY_MINUS, LatexParser::unary_minus);
    }
}



