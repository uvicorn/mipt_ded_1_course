#include <iostream>
#include <cassert>
#include <fstream>
#include "scene/scene.hpp"


int main(int argc, char** argv) {
    assert(argc >= 2);

    auto output_filename = argv[1];
    std::ofstream output_file;
    output_file.open(output_filename);

    Scene scene(output_file);
    scene.render_all();

    output_file.close();

    return 0;
}
