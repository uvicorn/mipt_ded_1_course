#include "differentiator.hpp"
#include <string>
#include <iostream>


int main(){
    while (true){
        std::string input;
        std::getline(std::cin, input);
        std::cout << "Expression: " << input << '\n';

        auto diff_output = Differentiator::FromString(input.c_str());
        for (auto& diff_by_var: diff_output){
            std::cout << "Differentiate by variable `" << diff_by_var.diff_var_name << "`\n";
            std::cout << "Result expression: " << diff_by_var.diff_expr_string << "\n\n";
        }
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n\n";
    }
}
