#ifndef DRAWER_H
#define DRAWER_H

#include <fstream>
#include "utils/types.hpp"
#include "math.h"

FORCE_INLINE Coord linear_to_gamma(Coord linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);

    return 0;
}

class Drawer{
    public:
        Coord image_width = 400;
        Coord aspect_ratio = 16.0 / 9.0;
        Coord image_height =  int(image_width / aspect_ratio);
        std::ofstream& output_stream;

        Drawer(); // TODO: проверить на пизду
        Drawer(std::ofstream& output_stream) :
            output_stream(output_stream)
        {

            // Calculate the image height, and ensure that it's at least 1.
            this->image_height = (image_height < 1) ? 1 : image_height;

            output_stream << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        }


        void write_color(const Color3& pixel_color) {
            auto r = pixel_color.x;
            auto g = pixel_color.y;
            auto b = pixel_color.z;
            
            // Apply a linear to gamma transform for gamma 2
            r = linear_to_gamma(r);
            g = linear_to_gamma(g);
            b = linear_to_gamma(b);
            
            static const Interval intensity(0.000, 0.999);
            int rbyte = int(256 * intensity.clamp(r));
            int gbyte = int(256 * intensity.clamp(g));
            int bbyte = int(256 * intensity.clamp(b));

            // Write out the pixel color components.
            output_stream << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
        }
};

#endif
