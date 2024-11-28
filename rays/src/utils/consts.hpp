#ifndef CONSTS_H
#define CONSTS_H

#include <limits>
// #include "structs/vec3.hpp"
#include "structs/vec3_simd.hpp"
// #include "types.hpp"

using Coord = SimdVec3::Coord;

const Coord Inf = std::numeric_limits<Coord>::infinity();
const Coord PI = 3.1415926535897932385;


#endif
