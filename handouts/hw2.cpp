#include "hw2.h"

Image3 hw_2_1(const std::vector<std::string> &params) {
    // Homework 2.1: render a single triangle and outputs
    // its barycentric coordinates.
    // We will use the following camera parameter
    // lookfrom = (0, 0,  0)
    // lookat   = (0, 0, -1)
    // up       = (0, 1,  0)
    // vfov     = 45
    // and we will parse the triangle vertices from params
    // The three vertices are stored in v0, v1, and v2 below.

    if (params.size() < 9) {
        // Not enough parameters to parse the triangle vertices.
        return Image3(0, 0);
    }

    Vector3 v0, v1, v2;
    v0.x = std::stof(params[i++]);
    v0.y = std::stof(params[i++]);
    v0.z = std::stof(params[i++]);
    v1.x = std::stof(params[i++]);
    v1.y = std::stof(params[i++]);
    v1.z = std::stof(params[i++]);
    v2.x = std::stof(params[i++]);
    v2.y = std::stof(params[i++]);
    v2.z = std::stof(params[i++]);

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}
