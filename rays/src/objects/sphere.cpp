#include "objects/sphere.hpp"
#include "ray.hpp"
#include <cmath>
// #include "math/equation_solver.hpp"


bool Sphere::hit(const Ray& ray, Interval ray_t, HitRecord& record) const {
    Vec3 sphere_norm_vec = center - ray.start_point;
    
    Coord a = ray.direction_vec.norm_square();
    Coord h = ray.direction_vec.dot(sphere_norm_vec);
    Coord c = sphere_norm_vec.norm_square() - radius*radius;

    Coord discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;

    Coord sqrtD = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtD) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtD) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    record.t = root;
    record.point = ray.at(record.t);

    Vec3 outward_normal = (record.point - center) / radius;
    record.set_face_normal(ray, outward_normal);

    return true;
}
