#ifndef EQUATION_H
#define EQUATION_H

#include "stddef.h"
#include <utility>

using Coef = long double;


bool quadratic_solver(Coef a, Coef b, Coef c, std::pair<Coef, Coef>& roots);

// size_t quadratic_num_solutions(Coef a, Coef b, Coef c);

bool quadratic_solver_inrange(Coef a, Coef b, Coef c, Coef rootmin, Coef root_max, std::pair<Coef, Coef>& roots);

#endif
