#include <iostream>
#include <cassert>
#include <fstream>
#include "scene/scene.hpp"
#include "objects/objects.hpp"


int main(int argc, char** argv) {
    assert(argc >= 2);

    auto output_filename = argv[1];
    std::ofstream output_file;
    output_file.open(output_filename);

    Scene scene(output_file);

    // scene.add(new Plane({1,2,2,2}));
    scene.add(new Triangle({{1,2,3},{1,2,4},{3,3,6}}));
    scene.add(new Triangle({{1,2,3},{1,2,4},{5,5,5}}));

    scene.render_all();

    output_file.close();

    return 0;
}
