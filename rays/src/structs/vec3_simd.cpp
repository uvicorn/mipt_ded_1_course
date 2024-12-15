#include "vec3_simd.hpp"
#include "utils/rand.hpp"
#include "math.h"

using Coord = SimdVec3::Coord;

inline std::ostream& operator<<(std::ostream& os, const SimdVec3& v) {
    return os  << v.x << ' '<< v.y << ' ' << v.z;
}

SimdVec3 SimdVec3::random(){
    return SimdVec3(random_coord(), random_coord(), random_coord());
}

SimdVec3 SimdVec3::random_normalized(){
    while (true) {
        auto p = SimdVec3::random(-1,1);
        auto lensq = p.norm_square();
        if (1e-160 < lensq && lensq <= 1)
            return p / std::sqrt(lensq);
    }
}

SimdVec3 SimdVec3::random(Coord min, Coord max){
    return SimdVec3(random_coord(min,max), random_coord(min,max), random_coord(min,max));
}

SimdVec3 SimdVec3::random_on_hemisphere(const SimdVec3& normal){
    SimdVec3 on_unit_sphere = SimdVec3::random_normalized();
    if (on_unit_sphere.dot(normal) > 0.0)
        return on_unit_sphere;
    return -on_unit_sphere;
}
