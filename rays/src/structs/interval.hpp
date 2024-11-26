#ifndef INTERVAL_H
#define INTERVAL_H

#include "utils/consts.hpp"

class Interval {
  public:
    Coord min, max;

    Interval() : min(+Inf), max(-Inf) {} // Default interval is empty

    Interval(Coord min, Coord max) : min(min), max(max) {}

    Coord length() const {
        return max - min;
    }

    bool contains(Coord x) const {
        return min <= x && x <= max;
    }

    bool surrounds(Coord x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
    static const Interval Empty, Universe;
};


#endif
