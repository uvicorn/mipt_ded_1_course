#ifndef FUNCS_H
#define FUNCS_H

#include "types.hpp"
#include "consts.hpp"

#include "rand.hpp"

inline Coord degrees_to_radians(Coord degrees) {
    return degrees * PI / 180.0;
}

#endif
