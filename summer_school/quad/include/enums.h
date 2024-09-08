#ifndef ENUMS_H
#define ENUMS_H

#include "stdio.h"

#define EPS 10e-6
#define INF_ROOTS -2
#define SOLVING_ERROR -1
#define INVALID_ARGUMENT -2

#define USER_INPUT_COUNT 3

enum Equation{
    QUAD,
    LINEAR,
    CONST,
    INFINITY_SOL_EQUATION
};

#define CHECK_ZERO_ADDR(x) if ( x == NULL ){ fprintf(stderr, "INVALID ADDRESS: NULL"); return INVALID_ARGUMENT;}
#define CHECK_ZERO_ADDR_SET_ROOTS(x,roots) if ( x == NULL ){ fprintf(stderr, "INVALID ADDRESS: NULL"); roots->solution_count = SOLVING_ERROR; return;}


#endif
