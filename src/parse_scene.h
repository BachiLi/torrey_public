#pragma once

#include "torrey.h"
#include "vector.h"

#include <filesystem>
#include <variant>
#include <vector>

struct ParsedCamera {
    Vector3 lookfrom;
    Vector3 lookat;
    Vector3 up;
    Real vfov;
    int width, height;
};

struct ParsedImageTexture {
    fs::path filename;
    Real uscale = 1, vscale = 1;
    Real uoffset = 0, voffset = 0;
};

using ParsedColor = std::variant<Vector3 /* RGB */, ParsedImageTexture>;

struct ParsedDiffuse {
    ParsedColor reflectance;
};

struct ParsedMirror {
    ParsedColor reflectance;
};

struct ParsedPlastic {
    Real eta; // index of refraction
    ParsedColor reflectance;
};

struct ParsedPhong {
    ParsedColor reflectance; // Ks
    Real exponent; // alpha
};

struct ParsedBlinnPhong {
    ParsedColor reflectance; // Ks
    Real exponent; // alpha
};

struct ParsedBlinnPhongMicrofacet {
    ParsedColor reflectance; // Ks
    Real exponent; // alpha
};

using ParsedMaterial = std::variant<ParsedDiffuse,
                                    ParsedMirror,
                                    ParsedPlastic,
                                    ParsedPhong,
                                    ParsedBlinnPhong,
                                    ParsedBlinnPhongMicrofacet>;

struct ParsedPointLight {
    Vector3 position;
    Vector3 intensity;
};

struct ParsedDiffuseAreaLight {
    int shape_id;
    Vector3 radiance;
};

using ParsedLight = std::variant<ParsedPointLight, ParsedDiffuseAreaLight>;

/// A Shape is a geometric entity that describes a surface. E.g., a sphere, a triangle mesh, a NURBS, etc.
/// For each shape, we also store an integer "material ID" that points to a material, and an integer
/// "area light ID" that points to a light source if the shape is an area light. area_lightID is set to -1
/// if the shape is not an area light.
struct ParsedShapeBase {
    int material_id = -1;
    int area_light_id = -1;
};

struct ParsedSphere : public ParsedShapeBase {
    Vector3 position;
    Real radius;
};

struct ParsedTriangleMesh : public ParsedShapeBase {
    std::vector<Vector3> positions;
    std::vector<Vector3i> indices;
    std::vector<Vector3> normals;
    std::vector<Vector2> uvs;
};

using ParsedShape = std::variant<ParsedSphere, ParsedTriangleMesh>;

inline void set_material_id(ParsedShape &shape, int material_id) {
    std::visit([&](auto &s) { s.material_id = material_id; }, shape);
}
inline void set_area_light_id(ParsedShape &shape, int area_light_id) {
    std::visit([&](auto &s) { s.area_light_id = area_light_id; }, shape);
}
inline int get_material_id(const ParsedShape &shape) {
    return std::visit([&](const auto &s) { return s.material_id; }, shape);
}
inline int get_area_light_id(const ParsedShape &shape) {
    return std::visit([&](const auto &s) { return s.area_light_id; }, shape);
}
inline bool is_light(const ParsedShape &shape) {
    return get_area_light_id(shape) >= 0;
}

struct ParsedScene {
    ParsedCamera camera;
    std::vector<ParsedMaterial> materials;
    std::vector<ParsedLight> lights;
    std::vector<ParsedShape> shapes;
    Vector3 background_color;
    int samples_per_pixel;
};

ParsedScene parse_scene(const fs::path &filename);
