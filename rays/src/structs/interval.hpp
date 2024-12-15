#ifndef INTERVAL_H
#define INTERVAL_H

#include "utils/consts.hpp"
#include "utils/macro.hpp"

class Interval {
  public:
    Coord min, max;

    Interval() : min(+Inf), max(-Inf) {} // Default interval is empty

    Interval(Coord min, Coord max) : min(min), max(max) {}

    FORCE_INLINE Coord length() const {
        return max - min;
    }

    FORCE_INLINE bool contains(Coord x) const {
        return min <= x && x <= max;
    }

    FORCE_INLINE bool surrounds(Coord x) const {
        return min < x && x < max;
    }

    Coord clamp(Coord x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
    static const Interval Empty, Universe;
};


#endif
