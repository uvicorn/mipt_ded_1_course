#ifndef DRAWER_H
#define DRAWER_H

#include <fstream>
#include "utils/types.hpp"

class Drawer{
    public:
        Coord image_height;
        Coord image_width;
        std::ofstream& output_stream;

        Drawer(); // TODO: проверить на пизду
        Drawer(std::ofstream& output_stream) :
            output_stream(output_stream)
        {
            auto aspect_ratio = 16.0 / 9.0;
            this->image_width = 400;

            // Calculate the image height, and ensure that it's at least 1.
            int image_height = int(image_width / aspect_ratio);
            this->image_height = (image_height < 1) ? 1 : image_height;

            output_stream << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        }


        void write_color(const Color3& pixel_color) {
            auto r = pixel_color.x();
            auto g = pixel_color.y();
            auto b = pixel_color.z();
            
            
            static const Interval intensity(0.000, 0.999);
            int rbyte = int(256 * intensity.clamp(r));
            int gbyte = int(256 * intensity.clamp(g));
            int bbyte = int(256 * intensity.clamp(b));

            // Write out the pixel color components.
            output_stream << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
        }
};

#endif
