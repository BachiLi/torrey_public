#pragma once

namespace hw1 {

struct Camera {
    Vector3 lookfrom;
    Vector3 lookat;
    Vector3 up;
    Real vfov;
};

struct SpherePrimitive {
    Vector3 center;
    Real radius;
    Vector3 diffuse_color;
};

struct PointLight {
    Vector3 intensity;
    Vector3 position;    
};

struct Scene {
    Camera camera;
    std::vector<SpherePrimitive> primitives;
    std::vector<PointLight> lights; 
};

Scene hw1_scene_0{
    Camera{
        Vector3{0, 0,  0}, // lookfrom
        Vector3{0, 0, -1}, // lookat
        Vector3{0, 1,  0}, // up
        90                 // vfov
    },
    std::vector<SpherePrimitive>{
        {Vector3{-1, 0, 3}, Real(0.7), Vector3{0.75, 0.25, 0.25}},
        {Vector3{ 1, 0, 3}, Real(0.7), Vector3{0.25, 0.25, 0.75}}
    },
    std::vector<PointLight>{
        {Vector3{1, 1, 1}, Vector3{0, 0, 0}}
    }
};

Scene hw1_scenes[] = {
    hw1_scene_0
};

}
