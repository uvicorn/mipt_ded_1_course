#ifndef RAND_HPP
#define RAND_HPP

#include <cstdlib>
#include "types.hpp"

template<typename T=Coord>
inline T random_coord() {
    // Returns a random real in [0,1).
    return std::rand() / (RAND_MAX + 1.0);
}

template<typename T=Coord>
inline T random_coord(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_coord();
}

#endif
