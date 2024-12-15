#ifndef FUNCS_H
#define FUNCS_H

#include "types.hpp"
#include "consts.hpp"
#include "utils/macro.hpp"
#include "rand.hpp"

FORCE_INLINE Coord degrees_to_radians(Coord degrees) {
    return degrees * PI / 180.0;
}


#define IS_ZERO(x) ((x)<1e-160)
#define IS_ZERO_ABS(x) (std::fabs(x)<1e-160)


#endif
