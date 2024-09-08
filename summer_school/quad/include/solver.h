#ifndef SOLVER_H
#define SOLVER_H

#include "enums.h"
#include "structs.h"

void solve_equation(struct Coefs* coefs, struct Roots* roots);
enum Equation match_equation_type(struct Coefs *coefs);
void solve_quadratic_equation(const struct Coefs *coefs, struct Roots* roots);
int solve_linear_equation(long double k, long double b, long double* x);


#endif
