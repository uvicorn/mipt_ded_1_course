#ifndef CONSTS_H
#define CONSTS_H

#include <limits>
// #include "structs/vec3.hpp"
#include "structs/vec3_simd.hpp"
// #include "types.hpp"
#include "utils/macro.hpp"

using Coord = SimdVec3::Coord;

constexpr Coord Inf = std::numeric_limits<Coord>::infinity();
constexpr Coord PI = 3.1415926535897932385;
constexpr Coord Eps = std::numeric_limits<Coord>::epsilon();

#endif
