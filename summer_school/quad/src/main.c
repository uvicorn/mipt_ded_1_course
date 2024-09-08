#include "solver.h"
#include "io.h"
#include "stdio.h"
#include "math.h"
#include "structs.h"
#include "enums.h"

#define __DEBUG 0


int main(){
    #if __DEBUG
    #include "tests/test_solver.c"
    #endif

    struct Coefs coefs = { .a = NAN, .b = NAN, .c = NAN};
    for (int iteration = 0; iteration < USER_INPUT_COUNT; iteration++){
        if (read_coefficients(&coefs) != 0) puts("Read coefs error, try again\n");
        else break;
    }
    struct Roots roots = {NAN, NAN};
    solve_equation(&coefs, &roots);

    print_solution(&roots);
    return 0;
}
