#pragma once

struct SpherePrimitive {
    Vector3 center;
    Real radius;
    Vector3 diffuse_color;
};

struct PointLight {
    Vector3 intensity;
    Vector3 position;    
};

struct HW1Scene {
    std::vector<SpherePrimitive> primitives;
    std::vector<PointLight> lights; 
};

HW1Scene hw1_scene_0{
    std::vector<SpherePrimitive>{
        {Vector3{-1, 0, 3}, Real(0.7), Vector3{0.75, 0.25, 0.25}},
        {Vector3{ 1, 0, 3}, Real(0.7), Vector3{0.25, 0.25, 0.75}}
    },
    std::vector<PointLight>{
        {Vector3{1, 1, 1}, Vector3{0, 0, 0}}
    };
};

std::vector<SpherePrimitive> hw1_scenes[] = {
    hw1_scene_0
};
