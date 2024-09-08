#include "structs.h"
#include "math.h"
#include "solver.h"
#include "stdio.h"
#include <stdbool.h>


enum Equation match_equation_type(struct Coefs *coefs){
    if ( coefs->a == 0 && coefs->b == 0 && coefs->c == 0 ) return INFINITY_SOL_EQUATION;
    if ( coefs->a == 0 ){
        if ( coefs->b != 0 ) 
            return LINEAR;
        else 
            return CONST;
    }
    return QUAD;
}

static inline bool is_lower_epsilon(long double x){
    return fabs(x) < EPS;
}

static inline void set_zero_lower_eps(long double* x){
    if (is_lower_epsilon(*x)) *x = 0;
}
/*
 * writes solutions to x1 and x2
*/
void solve_quadratic_equation(const struct Coefs *coefs, struct Roots* roots){
    if (roots==NULL)return;
    CHECK_ZERO_ADDR_SET_ROOTS(coefs,roots);

    long double D = coefs->b * coefs->b - 4*coefs->a*coefs->c;

    set_zero_lower_eps(&D);

    if ( D < 0 ){
        roots->solution_count = 0;
    } else if (D == 0) {
        roots->x1 = -coefs->b / (2 * coefs->a);

        roots->x2 = roots->x1;
        roots->solution_count = 1;
    } else {
        long double D_div_2a = sqrt(D) / (2*coefs->a);
        long double b_div_2a = -coefs->b / (2*coefs->a);

    roots->x1 = b_div_2a + D_div_2a;
        roots->x2 = b_div_2a - D_div_2a;
        roots->solution_count = 2;
    }
}


// solve kx+b = 0
int solve_linear_equation(long double k, long double b, long double* x){
    if (k == 0)
        return 0;
    else if (b == 0 && k != 0)
        *x = 0;
    else if (b == 0 && k == 0) 
        return INF_ROOTS;
    else 
        *x = -b/k;

    return 1;
}


void solve_equation(struct Coefs* coefs, struct Roots* roots){
    if (roots==NULL)return;
    CHECK_ZERO_ADDR_SET_ROOTS(coefs,roots);

    switch (match_equation_type(coefs)){
        case CONST:
            roots->solution_count = 0;
            break;
        case LINEAR:
            roots->solution_count = solve_linear_equation(coefs->b, coefs->c, &roots->x1);
            roots->x2             = roots->x1;
            break;
        case QUAD:
            solve_quadratic_equation(coefs, roots);
            break;
        case INFINITY_SOL_EQUATION:
            roots->solution_count = INF_ROOTS;
            break;
        default:
            roots->solution_count = SOLVING_ERROR;
            break;
    }
    set_zero_lower_eps(&roots->x1);
    set_zero_lower_eps(&roots->x2);
}
