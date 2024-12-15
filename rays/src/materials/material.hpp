#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "objects/hittable.hpp"
#include "utils/types.hpp"

class Material {
  public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& ray_in, const HitRecord& hit_record, Color3& attenuation, Ray& scattered
    ) const {
        return false;
    }
};

#endif
