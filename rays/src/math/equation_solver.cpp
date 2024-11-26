#include "equation_solver.hpp"
#include <cmath>


bool quadratic_solver(Coef a, Coef b, Coef c, std::pair<Coef, Coef>& roots){
    Coef D = b*b - 4*a*c;
    if (D>=0){
        Coef sqrtD = std::sqrt(D);
        roots.first = (-b+sqrtD)/(2*a);
        roots.second= (-b-sqrtD)/(2*a);
        return true;
    }
    return false;
}

// size_t quadratic_num_solutions(Coef a, Coef b, Coef c){
//     Coef D = b*b - 4*a*c;
//     return (D>0) + (D>=0); // ДА БЛЯТЬ fabs НУЖЕН
// }

// size_t quadratic_solver_inrange(Coef a, Coef b, Coef c, Coef rootmin, Coef root_max, std::pair<Coef, Coef>& roots);
