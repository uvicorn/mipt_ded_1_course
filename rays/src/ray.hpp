#ifndef RAY_H
#define RAY_H

#include "utils/types.hpp"
#include <iostream>


class Ray {
    public:
        const Vec3 direction_vec;
        const Point3 start_point;

        Ray() {};
        Ray(const Point3& start_point, const Vec3& direction) : start_point(start_point), direction_vec(direction) {};

        // r = r0 + a*t
        Point3 at(Coord t) const {
            return start_point + direction_vec * t;
        }
};


#endif
