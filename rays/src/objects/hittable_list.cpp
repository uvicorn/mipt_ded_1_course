#include "hittable_list.hpp"
#include "utils/consts.hpp"

// TODO: подумать, стоит ли хранить unique_ptr
void HittableList::add(SharedHittable object) {
    this->objects.push_back(object);
}


bool HittableList::hit(const Ray& ray, Interval ray_t, HitRecord& record) const{
    HitRecord temp_record;
    bool hit_anything = false;
    auto closest_so_far = ray_t.max;

    for (const auto object : objects) {
        if (object->hit(ray, Interval(ray_t.min, closest_so_far), temp_record)) {
            hit_anything = true; // TODO: останавливать ли цикл, когда во что-то врезались?
            closest_so_far = temp_record.t;
            record = temp_record;
        }
    }

    return hit_anything;
}


Color3 HittableList::ray_color(const Ray& ray, size_t depth) {
    HitRecord hit_record;
    if (!depth)
        return Color3(0,0,0);

    if (this->hit(ray, Interval(0.001, Inf), hit_record)) {
        // Vec3 direction = hit_record.normal + Vec3::random_normalized();
        Vec3 direction = Vec3::random_on_hemisphere(hit_record.normal);
        return 0.3 * ray_color(Ray(ray.start_point, direction), depth-1);
    }

    Vec3 unit_direction = ray.direction_vec.normalized();
    auto a = 0.5*(unit_direction.y + 1.0);
    return (1.0-a)*Color3(1.0, 1.0, 1.0) + a*Color3(0.5, 0.7, 1.0);
}

HittableList::~HittableList(){
    for (auto obj: objects)
        delete obj;
    objects.clear();
}
