#ifndef hittable_list_h
#define hittable_list_h

#include "objects/hittable.hpp"
#include <vector>
#include <memory>

// using SharedHittable = std::shared_ptr<Hittable>;
using SharedHittable = Hittable*;

class HittableList {
  public:
    std::vector<SharedHittable> objects;

    HittableList() = default;
    HittableList(SharedHittable object);
    HittableList(std::vector<SharedHittable> objects);
    ~HittableList();

    void add(SharedHittable object);
    void clear();
    bool hit(const Ray& ray, Interval ray_t, HitRecord& record) const;
    Color3 ray_color(const Ray& ray);
};


#endif
