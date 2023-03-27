#include "hw3.h"
#include "parse_scene.h"

Image3 hw_3_1(const std::vector<std::string> &params) {
    // Homework 3.1: image textures
    if (params.size() < 1) {
        return Image3(0, 0);
    }

    ParsedScene scene = parse_scene(params[0]);
    UNUSED(scene);

    return Image3(0, 0);
}

Image3 hw_3_2(const std::vector<std::string> &params) {
    // Homework 3.2: shading normals
    if (params.size() < 1) {
        return Image3(0, 0);
    }

    ParsedScene scene = parse_scene(params[0]);
    UNUSED(scene);

    return Image3(0, 0);
}

Image3 hw_3_3(const std::vector<std::string> &params) {
    // Homework 3.3: BRDFs
    if (params.size() < 1) {
        return Image3(0, 0);
    }

    ParsedScene scene = parse_scene(params[0]);
    UNUSED(scene);

    return Image3(0, 0);
}
