#ifndef latex_converter_HPP
#define latex_converter_HPP

#include "expr.hpp"
#include "visitors/visitors.hpp"
#include <iostream>
#include <fstream>


class LatexConverter{
    std::ofstream output_file;

    LatexConverter(char* output_filename){
        output_file.open(output_filename);
    }

    void dump(){
        
    }
};

#endif
