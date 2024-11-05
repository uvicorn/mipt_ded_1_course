#include "stddef.h"
#include <stdlib.h>

template<typename T>
class Matrix {
    public:
        T** elements;
        size_t columns;
        size_t rows;

    Matrix(size_t rows, size_t columns){
        this->elements = calloc(rows * columns * sizeof(T));
    }

    Matrix operator*(Matrix const& B){
        
    }
    /* Matrix operator*(int ){ */
        
    /* } */
};
