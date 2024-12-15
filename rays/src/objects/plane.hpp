#ifndef PLANE_HPP
#define PLANE_HPP

#include <algorithm>
#include <initializer_list>
#include "hittable.hpp"
#include "utils/types.hpp"
// #include "structs/matrix.hpp"
#include "utils/funcs.hpp"
#include <cassert>

class Plane : public Hittable {
  protected:
    union {
        struct {
            float A;
            float B;
            float C;
            float D;
        };
        float coefs[4];
        __m128 mmvalue;
    };
    Vec3 normal;
  public:
    // Plane(std::tuple<Vec3&, Vec3&> direct_vecs, Point3 start_point){
    // }
    Plane(const Coord coefs[4]) : mmvalue(_mm_set_ps(coefs[3], coefs[2], coefs[1], coefs[0])) {
        normal = Vec3(A,B,C);
    }
    Plane(const std::initializer_list<Coord> coefs){
        assert(coefs.size() == 4);

        std::copy_n(coefs.begin(), 4, this->coefs);
        normal = Vec3(A,B,C);
    }

    bool hit(const Ray& ray,Interval,HitRecord& record) const override{
        // ray.
        if (IS_ZERO(ray.direction_vec.dot(normal)))
            return false; // vectors are parallel

        record.set_face_normal(ray, normal);
        return true;
    }

};


#endif
