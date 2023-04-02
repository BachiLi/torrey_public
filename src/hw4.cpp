#include "hw4.h"
#include "parse_scene.h"

Image3 hw_4_1(const std::vector<std::string> &params) {
    // Homework 4.1: diffuse interreflection
    if (params.size() < 1) {
        return Image3(0, 0);
    }

    int max_depth = 50;
    std::string filename;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-max_depth") {
            max_depth = std::stoi(params[++i]);
        } else if (filename.empty()) {
            filename = params[i];
        }
    }

    ParsedScene scene = parse_scene(filename);
    UNUSED(scene);

    return Image3(0, 0);
}

Image3 hw_4_2(const std::vector<std::string> &params) {
    // Homework 4.2: adding more materials
    if (params.size() < 1) {
        return Image3(0, 0);
    }

    int max_depth = 50;
    std::string filename;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-max_depth") {
            max_depth = std::stoi(params[++i]);
        } else if (filename.empty()) {
            filename = params[i];
        }
    }

    ParsedScene scene = parse_scene(filename);
    UNUSED(scene);

    return Image3(0, 0);
}

Image3 hw_4_3(const std::vector<std::string> &params) {
    // Homework 4.3: multiple importance sampling
    if (params.size() < 1) {
        return Image3(0, 0);
    }

    int max_depth = 50;
    std::string filename;
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-max_depth") {
            max_depth = std::stoi(params[++i]);
        } else if (filename.empty()) {
            filename = params[i];
        }
    }

    ParsedScene scene = parse_scene(filename);
    UNUSED(scene);

    return Image3(0, 0);
}
