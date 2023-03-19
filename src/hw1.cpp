#include "hw1.h"
#include "hw1_scenes.h"

Image3 hw_1_1(const std::vector<std::string> &/*params*/) {
    // Homework 1.1: modify this function to 
    // generate camera rays and output the ray directions

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
    // Homework 1.2: modify this function to 
    // intersect the rays and a unit sphere located at (0, 0, 2)

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
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
