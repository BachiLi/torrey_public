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

    std::vector<float> tri_params;
    int spp = 16;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-spp") {
            spp = std::stoi(params[++i]);
        } else {
            tri_params.push_back(std::stof(params[i]));
        }
    }

    if (tri_params.size() < 9) {
        // Not enough parameters to parse the triangle vertices.
        return Image3(0, 0);
    }

    Vector3 p0{tri_params[0], tri_params[1], tri_params[2]};
    Vector3 p1{tri_params[3], tri_params[4], tri_params[5]};
    Vector3 p2{tri_params[6], tri_params[7], tri_params[8]};

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}
