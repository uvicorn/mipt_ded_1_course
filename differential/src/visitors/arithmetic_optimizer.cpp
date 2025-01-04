#include "tokenizer.hpp"
#include "visitors.hpp"
#include "helper.hpp"

// https://stackoverflow.com/questions/43532261/clean-way-to-simplify-a-binary-expression-tree
void Visitors::ArithmeticOptimizer(Expr::Expr* root){
    if (root == nullptr)
        return;

    std::visit(overloaded{
        [](const Expr::Number& expr) mutable {},
        [](const Expr::Identifier& expr) mutable {},

        [root](const Expr::Binary& expr) mutable {
            using Tokenizer::TokenType;

            ArithmeticOptimizer(expr.left);
            ArithmeticOptimizer(expr.right);

            bool left_is_num = Expr::check_expr_type<Expr::Number>(*expr.left);
            bool right_is_num = Expr::check_expr_type<Expr::Number>(*expr.right);
            Tokenizer::NumValue left_num = {};
            Tokenizer::NumValue right_num = {};
            if (left_is_num){
                left_num = std::get<Expr::Number>(expr.left->kind).value;
            }
            if (right_is_num){
                right_num = std::get<Expr::Number>(expr.right->kind).value;
            }
            bool use_left = true;
            bool use_right= true;

            switch (expr.op.type){
                case TokenType::PLUS:
                    if (left_is_num && left_num == 0)
                        *root = *expr.right;
                    else if (right_is_num && right_num == 0)
                        *root = *expr.left;
                    break;

                case TokenType::MINUS:
                    if (left_is_num && left_num == 0)
                        *root = *expr.right;
                    else if (right_is_num && right_num == 0)
                        *root = Expr::Expr(Expr::Unary(TokenType::MINUS, expr.left));
                    break;

                case TokenType::MUL:
                    if (left_is_num){
                        if (left_num == 0)
                            *root = Expr::Expr(Expr::Number(0));
                        else if (left_num == 1)
                            *root = *expr.right;
                    }
                    if (right_is_num){
                        if (right_num == 0)
                            *root = Expr::Expr(Expr::Number(0));
                        else if (right_num == 1)
                            *root = *expr.left;
                    }
                    break;
                case TokenType::DIV:
                    if (right_is_num){
                        if (right_num == 0)
                            assert(0 && "NO ZERO DIVISION");
                        else if (right_num == 1)
                            *root = *expr.left;
                    }
                    break;
                default:
                    assert(0 && !"NO SUCH TOKENTYPE IN ArithmeticOptimizer");
            }
            // return new Expr::Expr(Expr::Binary(new_left, expr.op, new_right));
        },

        [root](const Expr::Unary& expr) mutable {
            ArithmeticOptimizer(expr.right);

            if (expr.op.type == Expr::TokenType::PLUS){
                *root = *expr.right;
            }

            else if (Expr::check_expr_type<Expr::Unary>(*expr.right)){
                auto child_node = std::get<Expr::Unary>(expr.right->kind);
                if (child_node.op.type == Expr::TokenType::MINUS){
                    *root = *child_node.right;
                }
            }
        },

        [](const Expr::Call& expr) mutable {
        // TODO
        },

        [root](const Expr::Grouping& expr) mutable {
            // Expr::Expr* save_root = root;
            ArithmeticOptimizer(expr.expr);
            *root = *expr.expr;
            // delete save_root;
        }
    }, (root)->kind);
}
