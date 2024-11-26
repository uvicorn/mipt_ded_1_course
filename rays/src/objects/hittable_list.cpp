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


Color3 HittableList::ray_color(const Ray& ray) {
    HitRecord hit_record;
    if (this->hit(ray, Interval(0, Inf), hit_record)) {
        return 0.5 * (hit_record.normal + Color3(1,1,1));
    }

    Vec3 unit_direction = ray.direction_vec.normalized();
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*Color3(1.0, 1.0, 1.0) + a*Color3(0.5, 0.7, 1.0);
}

HittableList::~HittableList(){
    for (auto obj: objects)
        delete obj;
    objects.clear();
}
