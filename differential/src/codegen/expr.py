import sys

output_file = open(sys.argv[1], 'w')

out = ''
# DISCLAIMER = "// CODE WAS GENERATED WITH CHATGPT DONT TOUCH\n"*4
DISCLAIMER = ''

out += DISCLAIMER

out += \
'''#ifndef EXPR_HPP_CODEGEN
#define EXPR_HPP_CODEGEN

#include "tokenizer.hpp"
#include <vector>

namespace Expr{

using namespace Tokenizer;


class Expr{};

'''


def DEFINE_NODE_CLASS(classname, fields):
    out = f'class {classname} : public Expr' + '{\n'
    out += '  public:\n'
    ctor = f"    {classname}("+', '.join(fields) + '):\n'

    for field in fields:
        type, name = field.split()
        out += ' '*4+field+';\n'
        if 'vector' in type:
            ctor += ' '*8+name+'(std::move('+name+')),\n' # vec(std::move(field))
        else:
            ctor += ' '*8+name+'('+name+'),\n' # field(field)
    ctor = ctor[:-2]+'\n'+' '*4+'{};\n'
    out += ctor
    out +='};\n\n'

    return out

arr = [
    ('Identifier', ['LiteralType name']),
    ("Number", ["NumValue value"]),
    ("Binary", ["Expr left", "Token op", "Expr right"]),
    ("Unary", ["Token op", "Expr right"]),
    ("Grouping", ["Expr expr"]),
    # ("Call", ["Expr callee", "Token right_paren", "std::vector<Expr>& args"])
]
out += \
'''
class Call : public Expr{
  public:
    Expr callee;
    Token right_paren;
    std::vector<Expr> args;
    Call(Expr callee, Token right_paren, std::vector<Expr>& args):
        callee(callee),
        right_paren(right_paren),
        args(std::move(args))
    {};
};
'''

for i in arr:
    out+=DEFINE_NODE_CLASS(*i)

out += DISCLAIMER
out += "};\n#endif"


output_file.write(out)
output_file.close()
