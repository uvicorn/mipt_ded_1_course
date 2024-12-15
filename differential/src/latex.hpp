#ifndef LATEX_HPP
#define LATEX_HPP

#include "tree.hpp"
#include <string>

std::string ToString(Node*);

namespace LatexParser {
    #define DEFINE_ONE_ARG_OP(OP_NAME, OP_STRING)                                       \
        inline std::string OP_NAME(Node* root){                                         \
            return (OP_STRING) + std::string("(") + ToString(root->childs[0])  + ")";   \
        }                                                                               
    #define DEFINE_TWO_ARGS_OP(OP_NAME, OP_STRING)                                      \
        inline std::string OP_NAME(Node* root){                                         \
            return ToString(root->childs[0]) + (OP_STRING) + ToString(root->childs[1]); \
        }

    DEFINE_ONE_ARG_OP(ln, "ln");
    // DEFINE_ONE_ARG_OP(sin, "sin");
    // DEFINE_ONE_ARG_OP(cos, "cos");
    DEFINE_ONE_ARG_OP(unary_minus, "-");

    DEFINE_TWO_ARGS_OP(mul, "\\cdot");
    DEFINE_TWO_ARGS_OP(sub, "-");
    DEFINE_TWO_ARGS_OP(add, "+");
    DEFINE_TWO_ARGS_OP(div, "/");
}

#endif
