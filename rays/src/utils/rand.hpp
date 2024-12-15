#ifndef RAND_HPP
#define RAND_HPP

#include <cstdlib>
#include "types.hpp"
#include <random>
#include "utils/macro.hpp"

// FORCE_INLINE Coord random_coord() {
//     // Returns a random real in [0,1).
//     return std::rand() / (RAND_MAX + 1.0);
// }

// FORCE_INLINE Coord random_coord(Coord min, Coord max) {
//     // Returns a random real in [min,max).
//     return min + (max-min)*random_coord();
// }



FORCE_INLINE Coord random_coord(Coord min = 0.0, Coord max = 1.0) {
    static std::uniform_real_distribution<Coord> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

#endif
