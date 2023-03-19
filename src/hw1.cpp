#include "hw1.h"
#include "hw1_scenes.h"

Image3 hw_1_1(const std::vector<std::string> &/*params*/) {
    // Homework 1.1: generate camera rays and output the ray directions
    // The camera is positioned at (0, 0, 0), facing towards (0, 0, -1),
    // with an up vector (0, 1, 0) and a vertical field of view of 90 degree.

    Image3 img(640 /* width */, 480 /* height */);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = 
                Vector3{(x + Real(0.5)) / img.width,
                        (y + Real(0.5)) / img.height,
                        Real(0)};
        }
    }
    return img;
}

Image3 hw_1_2(const std::vector<std::string> &/*params*/) {
    // Homework 1.2: intersect the rays generated from hw_1_1
    // with a unit sphere located at (0, 0, -2)

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: add camera control to hw_1_2. 
    // We will use a look at transform:
    // The inputs are "lookfrom" (camera position),
    //                "lookat" (target),
    //                and the up vector
    // and the vertical field of view (in degrees).
    // If the user did not specify, fall back to the default
    // values below.
    // If you use the default values, it should render
    // the same image as hw_1_2.

    Vector3 lookfrom = Vector3{0, 0,  0};
    Vector3 lookat   = Vector3{0, 0, -2};
    Vector3 up       = Vector3{0, 1,  0};
    Real    vfov     = 90;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-lookfrom") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            Real z = std::stof(params[++i]);
            lookfrom = Vector3{x, y, z};
        } else if (params[i] == "-lookat") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            Real z = std::stof(params[++i]);
            lookat = Vector3{x, y, z};
        } else if (params[i] == "-up") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            Real z = std::stof(params[++i]);
            up = Vector3{x, y, z};
        } else if (params[i] == "-vfov") {
            vfov = std::stof(params[++i]);
        }
    }

    lookfrom; lookat; up; vfov; // avoid unused warnings

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.3: modify this function to 
    // intersect the rays and the spheres defined in hw1_scenes.h
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    int scene_id = std::stoi(params[0]);
    scene_id; // avoid unused warning
    // Your scene is hw1_scenes[scene_id]

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}
