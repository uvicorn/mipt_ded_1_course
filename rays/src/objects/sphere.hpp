#ifndef SPHERE_H
#define SPHERE_H
#include "ray.hpp"
#include "hittable.hpp"
#include "structs/vec3.hpp"
#include <cmath>

class Sphere : public Hittable {
  public:
    Sphere(const Point3& center, double radius) : center(center), radius(std::fmax(0,radius)) {}
    bool hit(const Ray&,Interval,HitRecord&) const override;

  private:
    Point3 center;
    Coord radius;
};

// Coord hit_sphere(const Point3& center, Coord radius, const Ray& ray);

#endif
