#ifndef HITTT_H
#define HITTT_H

#include "ray.hpp"
#include <cstddef>
#include "structs/interval.hpp"

class HitRecord {
  public:
    Point3 point;
    Vec3 normal;
    Coord t;
    bool front_face;

    void set_face_normal(const Ray& ray, const Vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = ray.direction_vec.dot(outward_normal) < 0;
        this->normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& record) const = 0;
    // virtual Hittable* clone() const = default;
};


#endif
