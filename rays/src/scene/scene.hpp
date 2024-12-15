#ifndef SCENE_H
#define SCENE_H

#include "utils/types.hpp"
#include "objects/hittable_list.hpp"
#include "drawer.hpp"


class Scene{
    Point3 camera_center = Point3(0, 0, 0);
    HittableList world;
    Drawer drawer;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    Vec3 pixel00_loc;

    int samples_per_pixel = 100; // Count of random samples for each pixel
    Coord pixel_samples_scale = 1.0 / samples_per_pixel;  // Color scale factor for a sum of pixel samples
    int max_depth_bounces = 10; // Maximum number of ray bounces into scene

  public:
    Scene(std::ofstream& output_stream);
    void ctor_World();
    void add(SharedHittable);
    void ctor_Camera();
    void render_all();
    Vec3 sample_square() const;
    Ray get_ray(size_t i, size_t j) const;
};

#endif
