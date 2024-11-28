#include "scene.hpp"
#include "objects/sphere.hpp"
#include <iostream>
#include "utils/rand.hpp"

Scene::Scene(std::ofstream& output_stream):
    drawer(Drawer(output_stream))
{
    this->ctor_World();
    this->ctor_Camera();
}


void Scene::render_all(){
    for (size_t j = 0; j < drawer.image_height; j++) {
        for (size_t i = 0; i < drawer.image_width; i++) {
            Color3 pixel_color(0,0,0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                Ray ray = get_ray(i, j);
                pixel_color += this->world.ray_color(ray, max_depth_bounces);
            }
            this->drawer.write_color(pixel_samples_scale * pixel_color);
        }
    }

}

Ray Scene::get_ray(size_t i, size_t j) const {
    // Construct a camera ray originating from the origin and directed at randomly sampled
    // point around the pixel location i, j.

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
                      + ((i + offset.x) * pixel_delta_u)
                      + ((j + offset.y) * pixel_delta_v);

    auto ray_origin = camera_center;
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

Vec3 Scene::sample_square() const {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return Vec3(random_coord() - 0.5, random_coord() - 0.5, 0);
}


void Scene::ctor_Camera(){
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (Coord(drawer.image_width)/drawer.image_height);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vec3(viewport_width, 0, 0);
    auto viewport_v = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    this->pixel_delta_u = viewport_u / drawer.image_width;
    this->pixel_delta_v = viewport_v / drawer.image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center
                             - Vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    this->pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

}

// void Scene::ctor_Drawer(){
// }

void Scene::ctor_World(){
    // for (int i=-5;i<5;i++){
    //     this->world.add(new Sphere(Point3(i*1.25,0,-4), 1));
    // }
    this->world.add(new Sphere(Point3(0,0,-1), 0.5));
    this->world.add(new Sphere(Point3(0,-100.5,-1), 100));
}
