#include "objects/sphere.hpp"
#include "ray.hpp"
#include <cmath>
#include "math/equation_solver.hpp"


Coord hit_sphere(const Point3& center, Coord radius, const Ray& ray){
    Vec3 sphere_norm_vec = ray.start_point - center;
    
    auto a = ray.direction_vec.norm_square();
    auto h = -ray.direction_vec * sphere_norm_vec;
    auto c = sphere_norm_vec.norm_square() - radius*radius;

    auto discriminant = h*h - a*c;
    if (discriminant < 0){
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant) ) / a;
    }
}


bool Sphere::hit(const Ray& ray, Interval ray_t, HitRecord& record) const {
    Vec3 sphere_norm_vec = ray.start_point - center;
    
    auto a = ray.direction_vec.norm_square();
    auto h = -ray.direction_vec * sphere_norm_vec;
    auto c = sphere_norm_vec.norm_square() - radius*radius;
    
    auto discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;

    auto sqrtD = std::sqrt(discriminant);

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
