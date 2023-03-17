#include "hw1.h"

Image3 hw_1_1(const std::vector<std::string> &params) {
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

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: modify this function to 
    // intersect the rays and a unit sphere located at (0, 0, 2)

    Image3 img(640 /* width */, 480 /* height */);

    return img;
}
