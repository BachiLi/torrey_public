#pragma once

namespace hw1 {

enum class MaterialType {
    Diffuse,
    Mirror
};

struct Camera {
    Vector3 lookfrom;
    Vector3 lookat;
    Vector3 up;
    Real vfov;
};

struct Sphere {
    Vector3 center;
    Real radius;
    int material_id;
};

struct PointLight {
    Vector3 intensity;
    Vector3 position;    
};

struct Material {
    MaterialType type;
    Vector3 color;
};

struct Scene {
    Camera camera;
    std::vector<Sphere> shapes;
    std::vector<Material> materials;
    std::vector<PointLight> lights; 
};

Scene hw1_scene_0{
    Camera{
        Vector3{0, 0,  0}, // lookfrom
        Vector3{0, 0, -1}, // lookat
        Vector3{0, 1,  0}, // up
        45                 // vfov
    },
    std::vector<Sphere>{
        {Vector3{ 0.0,    0.0, -3.0},   0.5, 0},
        {Vector3{ 0.0, -100.5,  3.0}, 100.0, 1}
    },
    std::vector<Material>{
        {MaterialType::Diffuse, Vector3{0.75, 0.25, 0.25}},
        {MaterialType::Diffuse, Vector3{0.25, 0.25, 0.75}}
    },
    std::vector<PointLight>{
        {Vector3{100, 100, 100}, Vector3{5, 5, -2}}
    }
};

Scene hw1_scene_1{
    Camera{
        Vector3{0, 0,  0}, // lookfrom
        Vector3{0, 0, -1}, // lookat
        Vector3{0, 1,  0}, // up
        45                 // vfov
    },
    std::vector<Sphere>{
        {Vector3{ 0.0, -100.5, -3.0}, 100.0, 0},
        {Vector3{ 0.0,    0.0, -3.0},   0.5, 1},
        {Vector3{ 1.0,    0.0, -3.0},   0.5, 2},
        {Vector3{-1.0,    0.0, -3.0},   0.5, 3}
    },
    std::vector<Material>{
        {MaterialType::Diffuse, Vector3{0.80, 0.80, 0.20}},
        {MaterialType::Diffuse, Vector3{0.75, 0.25, 0.25}},
        {MaterialType::Mirror , Vector3{0.75, 0.25, 0.75}},
        {MaterialType::Mirror , Vector3{0.25, 0.75, 0.75}},
    },
    std::vector<PointLight>{
        {Vector3{100, 100, 100}, Vector3{ 5, 5,  2}},
        {Vector3{ 10,  10,  10}, Vector3{-5, 5,  1}},
        {Vector3{  2,   2,   2}, Vector3{ 0, 5, -5}}
    }
};

Scene hw1_scene_2{
    Camera{
        Vector3{0, 0,  0}, // lookfrom
        Vector3{0, 0, -1}, // lookat
        Vector3{0, 1,  0}, // up
        45                 // vfov
    },
    std::vector<Sphere>{
        {Vector3{ -0.75, 0.0,  -4.0}, Real(1)   , 0},
        {Vector3{   1.0, 0.0, -13.0}, Real(7.5) , 1},
        {Vector3{   0.5, 0.0,  -3.0}, Real(0.25), 2},
    },
    std::vector<Material>{
        {MaterialType::Mirror , Vector3{0.75, 0.75, 0.25}},
        {MaterialType::Diffuse, Vector3{0.25, 0.75, 0.75}},
        {MaterialType::Diffuse, Vector3{0.75, 0.25, 0.75}}
    },
    std::vector<PointLight>{
        {Vector3{100, 50, 50}, Vector3{ 1, 0, 10}},
        {Vector3{50, 50, 100}, Vector3{-1, 0, 10}},
    }
};

Scene hw1_scene_3{
    Camera{
        Vector3{0, 0,  0}, // lookfrom
        Vector3{0, 0, -1}, // lookat
        Vector3{0, 1,  0}, // up
        45                 // vfov
    },
    std::vector<Sphere>{
        {Vector3{   0.0,  0.0, 0.0}, Real(100)  , 0},
        {Vector3{ -0.35, 0.35,-3.5}, Real(0.25) , 1},
        {Vector3{  0.35, 0.35,-2.5}, Real(0.35) , 2},
        {Vector3{  0.35,-0.35,-2.0}, Real(0.3)  , 3},
        {Vector3{ -0.35,-0.35,-4.0}, Real(0.325), 4},
        {Vector3{  -1.5,  0.0,-3.0}, Real(0.5)  , 5},
        {Vector3{   1.5,  0.0,-3.0}, Real(0.5)  , 6},
        {Vector3{  10.0,  0.0,-3.0}, Real(0.5)  , 7},
        {Vector3{ -10.0,  0.0,-3.0}, Real(0.5)  , 7},
    },
    std::vector<Material>{
        {MaterialType::Diffuse, Vector3{ 0.5, 0.25, 0.25}},
        {MaterialType::Diffuse, Vector3{0.25,  0.5, 0.75}},
        {MaterialType::Diffuse, Vector3{0.75,  0.5, 0.25}},
        {MaterialType::Mirror , Vector3{0.25, 0.75,  0.5}},
        {MaterialType::Diffuse, Vector3{ 0.5, 0.75,  0.5}},
        {MaterialType::Mirror , Vector3{ 0.5,  0.5, 0.75}},
        {MaterialType::Diffuse, Vector3{ 0.5,  0.5, 0.75}},
        {MaterialType::Diffuse, Vector3{0.75, 0.75, 0.75}}
    },
    std::vector<PointLight>{
        {Vector3{10, 10, 10}, Vector3{0, 0, 0}},
        {Vector3{0.5, 0.5, 0.5}, Vector3{-0.4, 0.5, -3.0}},
        {Vector3{10000, 10000, 10000}, Vector3{0, 0, 90}},
    }
};

Scene hw1_scene_4{
    Camera{
        Vector3{0, 0,  0}, // lookfrom
        Vector3{0, 0, -1}, // lookat
        Vector3{0, 1,  0}, // up
        45                 // vfov
    },
    std::vector<Sphere>{
        {Vector3{0.781,2.293,-4.602}, 0.659, 0},
        {Vector3{-1.975,-1.115,-5.906}, 0.591, 1},
        {Vector3{-0.900,-0.518,-4.741}, 0.632, 2},
        {Vector3{-2.281,0.900,-4.271}, 0.392, 3},
        {Vector3{0.309,2.047,-6.365}, 0.550, 4},
        {Vector3{-1.281,2.314,-4.383}, 0.415, 5},
        {Vector3{0.229,-0.093,-3.150}, 0.331, 6},
        {Vector3{1.400,1.793,-3.364}, 0.322, 7},
        {Vector3{-0.795,-2.459,-6.424}, 0.563, 8},
        {Vector3{-1.045,-1.886,-4.895}, 0.418, 9},
        {Vector3{2.369,-0.402,-5.811}, 0.510, 10},
        {Vector3{-0.514,2.270,-2.799}, 0.635, 11},
        {Vector3{0.669,1.321,-7.112}, 0.300, 12},
        {Vector3{0.227,1.167,-5.870}, 0.299, 13},
        {Vector3{1.320,0.086,-7.343}, 0.510, 14},
        {Vector3{-1.410,-1.046,-2.946}, 0.280, 15},
        {Vector3{1.742,-1.488,-6.187}, 0.517, 16},
        {Vector3{-1.867,0.315,-4.901}, 0.747, 17},
        {Vector3{-0.269,0.208,-4.738}, 0.431, 18},
        {Vector3{0.966,0.860,-5.417}, 0.290, 19},
        {Vector3{-2.345,1.133,-5.147}, 0.298, 20},
        {Vector3{1.924,1.527,-6.724}, 0.510, 21},
        {Vector3{-0.418,-1.651,-2.595}, 0.594, 22},
        {Vector3{-2.344,-0.415,-3.685}, 0.617, 23},
        {Vector3{0.238,0.990,-3.018}, 0.306, 24},
        {Vector3{-1.016,-2.252,-2.555}, 0.392, 25},
        {Vector3{1.761,1.176,-5.583}, 0.407, 26},
        {Vector3{1.995,2.143,-2.525}, 0.296, 27},
        {Vector3{1.645,-1.920,-3.988}, 0.361, 28},
        {Vector3{0.189,-0.262,-4.485}, 0.298, 29}
    },
    std::vector<Material>{
        {MaterialType::Diffuse, Vector3{0.020,0.660,0.021}},
        {MaterialType::Diffuse, Vector3{0.823,0.830,0.703}},
        {MaterialType::Diffuse, Vector3{0.471,0.540,0.414}},
        {MaterialType::Mirror , Vector3{0.997,0.048,0.431}},
        {MaterialType::Diffuse, Vector3{0.253,0.089,0.712}},
        {MaterialType::Diffuse, Vector3{0.664,0.884,0.069}},
        {MaterialType::Diffuse, Vector3{0.399,0.475,0.090}},
        {MaterialType::Diffuse, Vector3{0.360,0.298,0.956}},
        {MaterialType::Diffuse, Vector3{0.147,0.115,0.440}},
        {MaterialType::Diffuse, Vector3{0.881,0.312,0.609}},
        {MaterialType::Diffuse, Vector3{0.946,0.094,0.617}},
        {MaterialType::Mirror , Vector3{0.649,0.847,0.018}},
        {MaterialType::Diffuse, Vector3{0.994,0.240,0.637}},
        {MaterialType::Diffuse, Vector3{0.228,0.861,0.613}},
        {MaterialType::Diffuse, Vector3{0.442,0.546,0.580}},
        {MaterialType::Diffuse, Vector3{0.122,0.874,0.081}},
        {MaterialType::Diffuse, Vector3{0.954,0.575,0.910}},
        {MaterialType::Diffuse, Vector3{0.601,0.420,0.757}},
        {MaterialType::Diffuse, Vector3{0.340,0.136,0.233}},
        {MaterialType::Diffuse, Vector3{0.227,0.570,0.241}},
        {MaterialType::Mirror , Vector3{0.906,0.774,0.042}},
        {MaterialType::Mirror , Vector3{0.287,0.709,0.301}},
        {MaterialType::Diffuse, Vector3{0.895,0.787,0.824}},
        {MaterialType::Mirror , Vector3{0.011,0.395,0.117}},
        {MaterialType::Mirror , Vector3{0.781,0.390,0.375}},
        {MaterialType::Mirror , Vector3{0.155,0.873,0.695}},
        {MaterialType::Mirror , Vector3{0.276,0.751,0.104}},
        {MaterialType::Diffuse, Vector3{0.503,0.465,0.232}},
        {MaterialType::Diffuse, Vector3{0.264,0.794,0.280}},
        {MaterialType::Mirror , Vector3{0.036,0.548,0.363}}
    },
    std::vector<PointLight>{
        {Vector3{10, 10, 10}, Vector3{0, 0, 0}},
        {Vector3{50, 5, 5}, Vector3{5, 5, -5}},
    }
};

Scene hw1_scenes[] = {
    hw1_scene_0, hw1_scene_1, hw1_scene_2, hw1_scene_3, hw1_scene_4
};

}
