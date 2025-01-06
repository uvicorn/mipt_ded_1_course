#include "tokenizer.hpp"
#include "visitors.hpp"
#include "helper.hpp"
#include <variant>


using Tokenizer::TokenType;

// https://stackoverflow.com/questions/43532261/clean-way-to-simplify-a-binary-expression-tree
Expr::Expr* Visitors::ArithmeticOptimizer(Expr::Expr* root){
    if (root == nullptr)
        return nullptr;

    if (
        auto* binary = std::get_if<Expr::Binary>(&root->kind)
    ){
        binary->left  = ArithmeticOptimizer(binary->left);
        binary->right = ArithmeticOptimizer(binary->right);

        bool left_is_num = Expr::check_expr_type<Expr::Number>(*binary->left);
        bool right_is_num = Expr::check_expr_type<Expr::Number>(*binary->right);

        Tokenizer::NumValue left_num = -1;
        Tokenizer::NumValue right_num = -1;
        if (left_is_num){
            left_num = std::get<Expr::Number>(binary->left->kind).value;
        }
        if (right_is_num){
            right_num = std::get<Expr::Number>(binary->right->kind).value;
        }

        Expr::Expr* const right_node = binary->right;
        Expr::Expr* const left_node  = binary->left;

        // Expr::Expr* child_node = nullptr;
        switch (binary->op.type){
            case TokenType::PLUS:
                if (left_num == 0){
                    delete binary->left;
                    delete root;
                    return ArithmeticOptimizer(right_node);
                }
                else if (right_num == 0){
                    delete binary->right;
                    delete root;
                    return ArithmeticOptimizer(left_node);
                }
                if (left_is_num && right_is_num){
                    TreeDestroyer(root);
                    return new Expr::Expr(Expr::Number(left_num + right_num));
                }
                break;

            case TokenType::MINUS:
                if (left_num == 0){
                    delete binary->left;
                    delete root;
                    Expr::Expr* child_node = new Expr::Expr(Expr::Unary(TokenType::MINUS, right_node));
                    return ArithmeticOptimizer(child_node);
                }
                else if (right_num == 0){
                    delete binary->right;
                    delete root;
                    return ArithmeticOptimizer(left_node);
                }
                if (left_is_num && right_is_num){
                    TreeDestroyer(root);
                    return new Expr::Expr(Expr::Number(left_num - right_num));
                }

            case TokenType::MUL:
                if (left_num == 0 || right_num == 0){
                    TreeDestroyer(root);
                    return new Expr::Expr(Expr::Number(0));
                }
                else if (left_num == 1){
                    delete binary->left;
                    delete root;
                    return ArithmeticOptimizer(right_node);
                }
                else if (right_num == 1){
                    delete binary->right;
                    delete root;
                    return ArithmeticOptimizer(left_node);
                }

                if (left_is_num && right_is_num){
                    Expr::Expr* child_node = new Expr::Expr(Expr::Number(left_num * right_num));
                    TreeDestroyer(root);
                    return child_node;
                }
                break;

            case TokenType::DIV:
                if (right_is_num){
                    if (right_num == 0){
                        assert(0 && "NO ZERO DIVISION");
                    }
                    else if (right_num == 1){
                        delete binary->right;
                        delete root;
                        return ArithmeticOptimizer(left_node);
                    }
                }
                break;
            default:
                assert(0 && !"NO SUCH TOKENTYPE IN ArithmeticOptimizer");
        }
    }

    else if (
        auto* unary = std::get_if<Expr::Unary>(&root->kind)
    ){
        unary->right = ArithmeticOptimizer(unary->right);

        // +a -> a
        if (unary->op.type == Expr::TokenType::PLUS){
            auto child_node = ArithmeticOptimizer(unary->right);
            delete root;
            return child_node;
        }

        // --a -> a
        else if (Expr::check_expr_type<Expr::Unary>(*unary->right)){
            auto child_unary = std::get<Expr::Unary>(unary->right->kind);
            if (child_unary.op.type == Expr::TokenType::MINUS){
                auto result = ArithmeticOptimizer(child_unary.right);
                delete unary->right;
                delete root;
                return result;
            }
        }
    }
    else if (
        auto* grouping = std::get_if<Expr::Grouping>(&root->kind)
    ){
        // (a) -> a
        auto expr = ArithmeticOptimizer(grouping->expr);
        delete root;
        return expr;
    }
    return root;
}
