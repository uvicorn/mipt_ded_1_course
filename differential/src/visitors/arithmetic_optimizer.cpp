#include "tokenizer.hpp"
#include "visitors.hpp"
#include "helper.hpp"
#include <variant>


using Tokenizer::TokenType;

// https://stackoverflow.com/questions/43532261/clean-way-to-simplify-a-binary-expression-tree
Expr::Expr* Visitors::ArithmeticOptimizer(Expr::Expr* root){
    if (root == nullptr)
        return nullptr;

    if (Expr::check_expr_type<Expr::Binary>(*root)){
        auto binary = Expr::get_expr_kind<Expr::Binary>(*root);

        binary.left  = ArithmeticOptimizer(binary.left);
        binary.right = ArithmeticOptimizer(binary.right);

        bool left_is_num = Expr::check_expr_type<Expr::Number>(*binary.left);
        bool right_is_num = Expr::check_expr_type<Expr::Number>(*binary.right);

        Tokenizer::NumValue left_num = {};
        Tokenizer::NumValue right_num = {};
        if (left_is_num){
            left_num = std::get<Expr::Number>(binary.left->kind).value;
        }
        if (right_is_num){
            right_num = std::get<Expr::Number>(binary.right->kind).value;
        }

        bool use_left = true;
        bool use_right= true;
        
        Expr::Expr* child_node = nullptr;
        switch (binary.op.type){
            case TokenType::PLUS:
                if (left_is_num && left_num == 0)
                    child_node = binary.right;
                else if (right_is_num && right_num == 0)
                    child_node = binary.left;
                else
                    break;

                delete root;
                return ArithmeticOptimizer(child_node);

            case TokenType::MINUS:
                if (left_is_num && left_num == 0)
                    child_node = binary.right;
                else if (right_is_num && right_num == 0)
                    child_node = new Expr::Expr(Expr::Unary(TokenType::MINUS, binary.left));
                else
                    break;

                delete root;
                return ArithmeticOptimizer(child_node);

            case TokenType::MUL:
                if (left_is_num){
                    if (left_num == 0)
                        child_node = new Expr::Expr(Expr::Number(0));
                    else if (left_num == 1)
                        child_node = binary.right;
                    else
                        goto MUL_RIGHT_CHECK;

                    delete root;
                    return ArithmeticOptimizer(child_node);
                }

                MUL_RIGHT_CHECK:
                if (right_is_num){
                    if (right_num == 0)
                        child_node = new Expr::Expr(Expr::Number(0));
                    else if (right_num == 1)
                        child_node = binary.left;
                    else
                        goto MUL_TWO_NUMS;

                    delete root;
                    return ArithmeticOptimizer(child_node);
                }

                MUL_TWO_NUMS:
                if (left_is_num && right_is_num){
                    return new Expr::Expr(Expr::Number(left_num * right_num));
                }

                break;

            case TokenType::DIV:
                if (right_is_num){
                    if (right_num == 0)
                        assert(0 && "NO ZERO DIVISION");
                    else if (right_num == 1)
                        child_node = binary.left;
                    else
                        goto DIV_BREAK;

                    delete root;
                    return ArithmeticOptimizer(child_node);
                }

                DIV_BREAK:
                break;
            default:
                assert(0 && !"NO SUCH TOKENTYPE IN ArithmeticOptimizer");
        }
    }

    else if (
        auto* unary = std::get_if<Expr::Unary>(&root->kind)
    ){
        unary->right = ArithmeticOptimizer(unary->right);

        if (unary->op.type == Expr::TokenType::PLUS){
            auto child_node = ArithmeticOptimizer(unary->right);
            delete root;
            return child_node;
        }

        else if (Expr::check_expr_type<Expr::Unary>(*unary->right)){
            auto child_node = std::get<Expr::Unary>(unary->right->kind);
            if (child_node.op.type == Expr::TokenType::MINUS){
                auto ret = ArithmeticOptimizer(child_node.right);
                delete root;
                return ret;
            }
        }
    }
    else if (
        auto* grouping = std::get_if<Expr::Grouping>(&root->kind)
    ){
        auto ret = ArithmeticOptimizer(grouping->expr);
        delete root;
        return ret;
    }
    return root;
}
