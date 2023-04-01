#include "parse_scene.h"
#include "3rdparty/pugixml.hpp"
#include "compute_normals.h"
#include "flexception.h"
#include "parse_obj.h"
#include "parse_ply.h"
#include "parse_serialized.h"
#include "transform.h"
#include <map>
#include <regex>
#include <vector>

const Real c_default_fov = 45.0;
const int c_default_res = 256;
const std::string c_default_filename = "image.exr";

enum class FovAxis {
    X,
    Y,
    DIAGONAL,
    SMALLER,
    LARGER
};

struct ParsedLookAtXform {
    Vector3 lookfrom = Vector3{0, 0, 0};
    Vector3 lookat = Vector3{0, 0, -1};
    Vector3 up = Vector3{0, 1, 0};
};

inline Vector3 sRGB_to_RGB(const Vector3 &srgb) {
    // https://en.wikipedia.org/wiki/SRGB#From_sRGB_to_CIE_XYZ
    Vector3 rgb = srgb;
    for (int i = 0; i < 3; i++) {
        rgb[i] = rgb[i] <= Real(0.04045) ?
            rgb[i] / Real(12.92) :
            pow((rgb[i] + Real(0.055)) / Real(1.055), Real(2.4));
    }
    return rgb;
}

std::vector<std::string> split_string(const std::string &str, const std::regex &delim_regex) {
    std::sregex_token_iterator first{begin(str), end(str), delim_regex, -1}, last;
    std::vector<std::string> list{first, last};
    return list;
}

Vector3 parse_vector3(const std::string &value) {
    std::vector<std::string> list = split_string(value, std::regex("(,| )+"));
    Vector3 v;
    if (list.size() == 1) {
        v[0] = std::stof(list[0]);
        v[1] = std::stof(list[0]);
        v[2] = std::stof(list[0]);
    } else if (list.size() == 3) {
        v[0] = std::stof(list[0]);
        v[1] = std::stof(list[1]);
        v[2] = std::stof(list[2]);
    } else {
        Error("parse_vector3 failed");
    }
    return v;
}

auto parse_default_map(const std::string &value,
                       const std::map<std::string, std::string> &default_map) {
    if (value.length() > 0) {
        if (value[0] == '$') {
            auto it = default_map.find(value.substr(1));
            if (it == default_map.end()) {
                Error(std::string("Reference default variable ") + value + std::string(" not found."));
            }
            return it;
        }
    }
    return default_map.end();
}

void parse_default_map(pugi::xml_node node,
                       std::map<std::string, std::string> &default_map) {
    if (node.attribute("name")) {
        std::string name = node.attribute("name").value();
        if (node.attribute("value")) {
            std::string value = node.attribute("value").value();
            default_map[name] = value;
        }
    }
}

int parse_integer(const std::string &value,
                  const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return std::stoi(it->second);
    }
    return std::stoi(value);
}

Real parse_float(const std::string &value,
                 const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return std::stof(it->second);
    }
    return std::stof(value);
}

std::string parse_string(const std::string &value,
                         const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return it->second;
    }
    return value;
}

bool parse_boolean(const std::string &value) {
    if (value == "true") {
        return true;
    } else if (value == "false") {
        return false;
    } else {
        Error("parse_boolean failed");
        return false;
    }
}

bool parse_boolean(const std::string &value,
                   const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return parse_boolean(it->second);
    }
    return parse_boolean(value);    
}

Vector3 parse_vector3(const std::string &value,
                      const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return parse_vector3(it->second);
    }
    return parse_vector3(value);
}

Vector3 parse_srgb(const std::string &value) {
    Vector3 srgb;
    if (value.size() == 7 && value[0] == '#') {
        char *end_ptr = NULL;
        // parse hex code (#abcdef)
        int encoded = strtol(value.c_str()+1, &end_ptr, 16);
        if (*end_ptr != '\0') {
            Error(std::string("Invalid SRGB value: ") + value);
        }
        srgb[0] = ((encoded & 0xFF0000) >> 16) / 255.0f;
        srgb[1] = ((encoded & 0x00FF00) >> 8) / 255.0f;
        srgb[2] =  (encoded & 0x0000FF) / 255.0f;
    } else {
        Error(std::string("Unknown SRGB format: ") + value);
    }
    return srgb;
}

Vector3 parse_srgb(const std::string &value,
                   const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return parse_srgb(it->second);
    }
    return parse_srgb(value);
}

Matrix4x4 parse_matrix4x4(const std::string &value) {
    std::vector<std::string> list = split_string(value, std::regex("(,| )+"));
    if (list.size() != 16) {
        Error("parse_matrix4x4 failed");
    }

    Matrix4x4 m;
    int k = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m(i, j) = std::stof(list[k++]);
        }
    }
    return m;
}

Matrix4x4 parse_matrix4x4(const std::string &value,
                          const std::map<std::string, std::string> &default_map) {
    if (auto it = parse_default_map(value, default_map); it != default_map.end()) {
        return parse_matrix4x4(it->second);
    }
    return parse_matrix4x4(value);
}

Matrix4x4 parse_transform(pugi::xml_node node,
                          const std::map<std::string, std::string> &default_map) {
    Matrix4x4 tform = Matrix4x4::identity();
    for (auto child : node.children()) {
        std::string name = to_lowercase(child.name());
        if (name == "scale") {
            Real x = 1.0;
            Real y = 1.0;
            Real z = 1.0;
            if (!child.attribute("x").empty()) {
                x = parse_float(child.attribute("x").value(), default_map);
            }
            if (!child.attribute("y").empty()) {
                y = parse_float(child.attribute("y").value(), default_map);
            }
            if (!child.attribute("z").empty()) {
                z = parse_float(child.attribute("z").value(), default_map);
            }
            if (!child.attribute("value").empty()) {
                Vector3 v = parse_vector3(
                    child.attribute("value").value(), default_map);
                x = v.x; y = v.y; z = v.z;
            }
            tform = scale(Vector3{x, y, z}) * tform;
        } else if (name == "translate") {
            Real x = 0.0;
            Real y = 0.0;
            Real z = 0.0;
            if (!child.attribute("x").empty()) {
                x = parse_float(child.attribute("x").value(), default_map);
            }
            if (!child.attribute("y").empty()) {
                y = parse_float(child.attribute("y").value(), default_map);
            }
            if (!child.attribute("z").empty()) {
                z = parse_float(child.attribute("z").value(), default_map);
            }
            if (!child.attribute("value").empty()) {
                Vector3 v = parse_vector3(
                    child.attribute("value").value(), default_map);
                x = v.x; y = v.y; z = v.z;
            }
            tform = translate(Vector3{x, y, z}) * tform;
        } else if (name == "rotate") {
            Real x = 0.0;
            Real y = 0.0;
            Real z = 0.0;
            Real angle = 0.0;
            if (!child.attribute("x").empty()) {
                x = parse_float(child.attribute("x").value(), default_map);
            }
            if (!child.attribute("y").empty()) {
                y = parse_float(child.attribute("y").value(), default_map);
            }
            if (!child.attribute("z").empty()) {
                z = parse_float(child.attribute("z").value(), default_map);
            }
            if (!child.attribute("angle").empty()) {
                angle = parse_float(child.attribute("angle").value(), default_map);
            }
            tform = rotate(angle, Vector3(x, y, z)) * tform;
        } else if (name == "lookat") {
            Vector3 pos = parse_vector3(
                child.attribute("origin").value(), default_map);
            Vector3 target = parse_vector3(
                child.attribute("target").value(), default_map);
            Vector3 up = parse_vector3(
                child.attribute("up").value(), default_map);
            tform = look_at(pos, target, up) * tform;
        } else if (name == "matrix") {
            Matrix4x4 trans = parse_matrix4x4(
                std::string(child.attribute("value").value()), default_map);
            tform = trans * tform;
        }
    }
    return tform;
}

std::tuple<int /* width */, int /* height */, std::string /* filename */>
        parse_film(pugi::xml_node node, const std::map<std::string, std::string> &default_map) {
    int width = c_default_res, height = c_default_res;
    std::string filename = c_default_filename;

    for (auto child : node.children()) {
        std::string type = child.name();
        std::string name = child.attribute("name").value();
        if (name == "width") {
            width = parse_integer(child.attribute("value").value(), default_map);
        } else if (name == "height") {
            height = parse_integer(child.attribute("value").value(), default_map);
        } else if (name == "filename") {
            filename = parse_string(child.attribute("value").value(), default_map);
        }
    }
    return std::make_tuple(width, height, filename);
}

ParsedLookAtXform parse_lookat(pugi::xml_node node,
                               const std::map<std::string, std::string> &default_map) {
    ParsedLookAtXform lookat_xform;
    for (auto child : node.children()) {
        std::string name = to_lowercase(child.name());
        if (name == "lookat") {
            lookat_xform.lookfrom = parse_vector3(
                child.attribute("origin").value(), default_map);
            lookat_xform.lookat = parse_vector3(
                child.attribute("target").value(), default_map);
            lookat_xform.up = parse_vector3(
                child.attribute("up").value(), default_map);
        } else {
            Error(std::string("Only support LookAt transform in a sensor."));
        }
    }
    return lookat_xform;
}

std::tuple<ParsedCamera, std::string /* output filename */, int /* sample_count */>
        parse_sensor(pugi::xml_node node,
                     const std::map<std::string, std::string> &default_map) {
    ParsedLookAtXform lookat_xform; 
    Real fov = c_default_fov;

    int width = c_default_res, height = c_default_res;
    std::string filename = c_default_filename;
    FovAxis fov_axis = FovAxis::X;

    int sample_count = 16;

    std::string type = node.attribute("type").value();
    if (type == "perspective") {
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "fov") {
                fov = parse_float(child.attribute("value").value(), default_map);
            } else if (name == "toWorld" || name == "to_world") {
                lookat_xform = parse_lookat(child, default_map);
            } else if (name == "fovAxis" || name == "fov_axis") {
                std::string value = child.attribute("value").value();
                if (value == "x") {
                    fov_axis = FovAxis::X;
                } else if (value == "y") {
                    fov_axis = FovAxis::Y;
                } else if (value == "diagonal") {
                    fov_axis = FovAxis::DIAGONAL;
                } else if (value == "smaller") {
                    fov_axis = FovAxis::SMALLER;
                } else if (value == "larger") {
                    fov_axis = FovAxis::LARGER;
                } else {
                    Error(std::string("Unknown fovAxis value: ") + value);
                }
            }
        }
    } else {
        Error(std::string("Unsupported sensor: ") + type);
    }

    for (auto child : node.children()) {
        if (std::string(child.name()) == "film") {
            std::tie(width, height, filename) = parse_film(child, default_map);
        } else if (std::string(child.name()) == "sampler") {
            std::string name = child.attribute("type").value();
            if (name != "independent") {
                std::cerr << "Warning: the renderer currently only supports independent samplers." << std::endl;
            }
            for (auto grand_child : child.children()) {
                std::string name = grand_child.attribute("name").value();
                if (name == "sampleCount" || name == "sample_count") {
                    sample_count = parse_integer(
                        grand_child.attribute("value").value(), default_map);
                }
            }
        }
    }

    // convert to vertical FOV
    if (fov_axis == FovAxis::X ||
            (fov_axis == FovAxis::SMALLER && width < height) ||
            (fov_axis == FovAxis::LARGER && height < width)) {
        fov = degrees(2 * atan(
            tan(radians(fov) / 2) * height / Real(width)));
    } else if (fov_axis == FovAxis::DIAGONAL) {
        Real aspect = Real(height) / width;
        Real diagonal = 2 * tan(radians(fov) / 2);
        Real height = diagonal / sqrt(1 + 1 / (aspect * aspect));
        fov = degrees(2 * atan(height / 2));
    }

    return std::make_tuple(ParsedCamera{lookat_xform.lookfrom,
                                        lookat_xform.lookat,
                                        lookat_xform.up,
                                        fov,
                                        width, height},
                           filename,
                           sample_count);
}

ParsedColor parse_texture(pugi::xml_node node,
                          const std::map<std::string, std::string> &default_map) {
    std::string type = node.attribute("type").value();
    if (type == "bitmap") {
        std::string filename = "";
        Real uscale = 1;
        Real vscale = 1;
        Real uoffset = 0;
        Real voffset = 0;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "filename") {
                filename = parse_string(
                    child.attribute("value").value(), default_map);
            } else if (name == "uvscale") {
                uscale = vscale = parse_float(
                    child.attribute("value").value(), default_map);
            } else if (name == "uscale") {
                uscale = parse_float(
                    child.attribute("value").value(), default_map);
            } else if (name == "vscale") {
                vscale = parse_float(
                    child.attribute("value").value(), default_map);
            } else if (name == "uoffset") {
                uoffset = parse_float(
                    child.attribute("value").value(), default_map);
            } else if (name == "voffset") {
                voffset = parse_float(
                    child.attribute("value").value(), default_map);
            }
        }
        fs::path path(filename);
        if (path.is_relative()) {
            path = fs::current_path() / path;
        }
        return ParsedImageTexture{path,
            uscale, vscale, uoffset, voffset};
    }
    Error(std::string("Unknown texture type: ") + type);
    return ParsedColor{};
}

ParsedColor parse_color(
        pugi::xml_node node,
        std::map<std::string /* name id */, ParsedColor> &texture_map,
        const std::map<std::string, std::string> &default_map) {
    std::string type = node.name();
    if (type == "rgb") {
        return ParsedColor(parse_vector3(node.attribute("value").value(), default_map));
    } else if (type == "srgb") {
        Vector3 srgb = parse_srgb(node.attribute("value").value(), default_map);
        return ParsedColor(sRGB_to_RGB(srgb));
    } else if (type == "ref") {
        // referencing a texture
        std::string ref_id = node.attribute("id").value();
        auto t_it = texture_map.find(ref_id);
        if (t_it == texture_map.end()) {
            Error(std::string("Texture not found. ID = ") + ref_id);
        }
        return t_it->second;
    } else if (type == "texture") {
        return parse_texture(node, default_map);
    } else {
        Error(std::string("Unknown spectrum texture type:") + type);
        return ParsedColor(Vector3{0, 0, 0});
    }
}

Vector3 parse_intensity(pugi::xml_node node,
                        const std::map<std::string, std::string> &default_map) {
    std::string type = node.name();
    if (type == "rgb") {
        return parse_vector3(
            node.attribute("value").value(), default_map);
    } else if (type == "srgb") {
        Vector3 srgb = parse_srgb(
            node.attribute("value").value(), default_map);
        return sRGB_to_RGB(srgb);
    }
    return Vector3{1, 1, 1};
}

std::tuple<std::string /* ID */, ParsedMaterial> parse_bsdf(
        pugi::xml_node node,
        std::map<std::string /* name id */, ParsedColor> &texture_map,
        const std::map<std::string, std::string> &default_map,
        const std::string &parent_id = "") {
    std::string type = node.attribute("type").value();
    std::string id = parent_id;
    if (!node.attribute("id").empty()) {
        id = node.attribute("id").value();
    }
    if (type == "twosided") {
        // In torrey, all BSDFs are twosided.
        for (auto child : node.children()) {
            if (std::string(child.name()) == "bsdf") {
                return parse_bsdf(child, texture_map, default_map, id);
            }
        }
    } else if (type == "diffuse") {
        ParsedColor reflectance(Vector3{0.5, 0.5, 0.5});
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "reflectance") {
                reflectance = parse_color(
                    child, texture_map, default_map);
            }
        }
        return std::make_tuple(id, ParsedDiffuse{reflectance});
    } else if (type == "mirror") {
        ParsedColor reflectance(Vector3{1, 1, 1});
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "reflectance") {
                reflectance = parse_color(
                    child, texture_map, default_map);
            }
        }
        return std::make_tuple(id, ParsedMirror{reflectance});
    } else if (type == "plastic") {
        ParsedColor reflectance(Vector3{0.5, 0.5, 0.5});
        Real eta = 1.5;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "reflectance") {
                reflectance = parse_color(
                    child, texture_map, default_map);
            } else if (name == "ior" || name == "eta") {
                eta = parse_float(child.attribute("value").value(), default_map);
            }
        }
        return std::make_tuple(id, ParsedPlastic{eta, reflectance});
    } else if (type == "phong") {
        ParsedColor reflectance(Vector3{0.5, 0.5, 0.5});
        Real exponent = 5;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "reflectance") {
                reflectance = parse_color(
                    child, texture_map, default_map);
            } else if (name == "exponent" || name == "alpha") {
                exponent = parse_float(child.attribute("value").value(), default_map);
            }
        }
        return std::make_tuple(id, ParsedPhong{reflectance, exponent});
    } else if (type == "blinn" || type == "blinnphong") {
        ParsedColor reflectance(Vector3{0.5, 0.5, 0.5});
        Real exponent = 5;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "reflectance") {
                reflectance = parse_color(
                    child, texture_map, default_map);
            } else if (name == "exponent" || name == "alpha") {
                exponent = parse_float(child.attribute("value").value(), default_map);
            }
        }
        return std::make_tuple(id, ParsedBlinnPhong{reflectance, exponent});
    } else if (type == "blinn_microfacet" || type == "blinnphong_microfacet") {
        ParsedColor reflectance(Vector3{0.5, 0.5, 0.5});
        Real exponent = 5;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "reflectance") {
                reflectance = parse_color(
                    child, texture_map, default_map);
            } else if (name == "exponent" || name == "alpha") {
                exponent = parse_float(child.attribute("value").value(), default_map);
            }
        }
        return std::make_tuple(id, ParsedBlinnPhongMicrofacet{reflectance, exponent});
    } else {
        Error(std::string("Unknown BSDF: ") + type);
    }
    return std::make_tuple("", ParsedMaterial{});
}

ParsedLight parse_emitter(pugi::xml_node node,
                          const std::map<std::string, std::string> &default_map) {
    std::string type = node.attribute("type").value();
    if (type == "point") {
        Vector3 position = Vector3{0, 0, 0};
        Vector3 intensity = Vector3{1, 1, 1};
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "position") {
                if (!child.attribute("x").empty()) {
                    position.x = parse_float(child.attribute("x").value(), default_map);
                }
                if (!child.attribute("y").empty()) {
                    position.y = parse_float(child.attribute("y").value(), default_map);
                }
                if (!child.attribute("z").empty()) {
                    position.z = parse_float(child.attribute("z").value(), default_map);
                }
            } else if (name == "intensity") {
                intensity = parse_intensity(child, default_map);
            }
        }
        return ParsedPointLight{position, intensity};
    } else {
        Error(std::string("Unknown emitter: ") + type);
    }
}

ParsedShape parse_shape(pugi::xml_node node,
                        std::vector<ParsedMaterial> &materials,
                        std::map<std::string /* name id */, int /* index id */> &material_map,
                        std::map<std::string /* name id */, ParsedColor> &texture_map,
                        std::vector<ParsedLight> &lights,
                        const std::vector<ParsedShape> &shapes,
                        const std::map<std::string, std::string> &default_map) {
    // First, parse the material inside the shape and get the material ID.
    int material_id = -1;
    for (auto child : node.children()) {
        std::string name = child.name();
        if (name == "ref") {
            // A reference to a BSDF
            std::string name_value = child.attribute("name").value();
            pugi::xml_attribute id = child.attribute("id");
            if (id.empty()) {
                Error("Material reference id not specified.");
            }
            auto it = material_map.find(id.value());
            if (it == material_map.end()) {
                Error(std::string("Material reference ") + id.value() + std::string(" not found."));
            }
            material_id = it->second;
        } else if (name == "bsdf") {
            ParsedMaterial m;
            std::string material_name;
            std::tie(material_name, m) = parse_bsdf(
                child, texture_map, default_map);
            if (!material_name.empty()) {
                material_map[material_name] = materials.size();
            }
            material_id = materials.size();
            materials.push_back(m);
        }
    }

    ParsedShape shape;
    std::string type = node.attribute("type").value();
    if (type == "obj") {
        std::string filename;
        Matrix4x4 to_world = Matrix4x4::identity();
        bool face_normals = false;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "filename") {
                filename = parse_string(child.attribute("value").value(), default_map);
            } else if (name == "toWorld" || name == "to_world") {
                if (std::string(child.name()) == "transform") {
                    to_world = parse_transform(child, default_map);
                }
            } else if (name == "faceNormals" || name == "face_normals") {
                face_normals = parse_boolean(
                    child.attribute("value").value(), default_map);
            }
        }
        shape = parse_obj(filename, to_world);
        ParsedTriangleMesh &mesh = std::get<ParsedTriangleMesh>(shape);
        if (face_normals) {
            mesh.normals = std::vector<Vector3>{};
        } else {
            if (mesh.normals.size() == 0) {
                mesh.normals = compute_normals(mesh.positions, mesh.indices);
            }
        }
    } else if (type == "ply") {
        std::string filename;
        int shape_index = 0;
        Matrix4x4 to_world = Matrix4x4::identity();
        bool face_normals = false;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "filename") {
                filename = parse_string(child.attribute("value").value(), default_map);
            } else if (name == "toWorld" || name == "to_world") {
                if (std::string(child.name()) == "transform") {
                    to_world = parse_transform(child, default_map);
                }
            } else if (name == "shapeIndex" || name == "shape_index") {
                shape_index = parse_integer(child.attribute("value").value(), default_map);
            } else if (name == "faceNormals" || name == "face_normals") {
                face_normals = parse_boolean(
                    child.attribute("value").value(), default_map);
            }
        }
        shape = parse_ply(filename, to_world);
        ParsedTriangleMesh &mesh = std::get<ParsedTriangleMesh>(shape);
        if (face_normals) {
            mesh.normals = std::vector<Vector3>{};
        } else {
            if (mesh.normals.size() == 0) {
                mesh.normals = compute_normals(mesh.positions, mesh.indices);
            }
        }
    } else if (type == "serialized") {
        std::string filename;
        int shape_index = 0;
        Matrix4x4 to_world = Matrix4x4::identity();
        bool face_normals = false;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "filename") {
                filename = parse_string(child.attribute("value").value(), default_map);
            } else if (name == "toWorld" || name == "to_world") {
                if (std::string(child.name()) == "transform") {
                    to_world = parse_transform(child, default_map);
                }
            } else if (name == "shapeIndex" || name == "shape_index") {
                shape_index = parse_integer(child.attribute("value").value(), default_map);
            } else if (name == "faceNormals" || name == "face_normals") {
                face_normals = parse_boolean(
                    child.attribute("value").value(), default_map);
            }
        }
        shape = parse_serialized(filename, shape_index, to_world);
        ParsedTriangleMesh &mesh = std::get<ParsedTriangleMesh>(shape);
        if (face_normals) {
            mesh.normals = std::vector<Vector3>{};
        } else {
            if (mesh.normals.size() == 0) {
                mesh.normals = compute_normals(mesh.positions, mesh.indices);
            }
        }
    } else if (type == "sphere") {
        Vector3 center{0, 0, 0};
        Real radius = 1;
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "center") {
                center = Vector3{
                    parse_float(child.attribute("x").value(), default_map),
                    parse_float(child.attribute("y").value(), default_map),
                    parse_float(child.attribute("z").value(), default_map)};
            } else if (name == "radius") {
                radius = parse_float(child.attribute("value").value(), default_map);
            }
        }
        shape = ParsedSphere{{}, center, radius};
    } else if (type == "rectangle") {
        // Create a triangle mesh
        Matrix4x4 to_world = Matrix4x4::identity();
        bool flip_normals = false;
        ParsedTriangleMesh mesh;
        mesh.positions = {
            Vector3{-1, -1, 0}, Vector3{ 1, -1, 0}, Vector3{ 1, 1, 0}, Vector3{-1, 1, 0}
        };
        mesh.indices = {
            Vector3i{0, 1, 2}, Vector3i{0, 2, 3}
        };
        mesh.uvs = {
            Vector2{0, 0}, Vector2{1, 0}, Vector2{1, 1}, Vector2{0, 1}
        };
        mesh.normals = {
            Vector3{0, 0, 1}, Vector3{0, 0, 1}, Vector3{0, 0, 1}, Vector3{0, 0, 1}
        };
        for (auto child : node.children()) {
            std::string name = child.attribute("name").value();
            if (name == "toWorld" || name == "to_world") {
                if (std::string(child.name()) == "transform") {
                    to_world = parse_transform(child, default_map);
                }
            } else if (name == "flipNormals" || name == "flip_normals") {
                flip_normals = parse_boolean(child.attribute("value").value(), default_map);
            }
        }
        if (flip_normals) {
            for (auto &n : mesh.normals) {
                n = -n;
            }
        }
        for (auto &p : mesh.positions) {
            p = xform_point(to_world, p);
        }
        for (auto &n : mesh.normals) {
            n = xform_normal(inverse(to_world), n);
        }
        shape = mesh;
    } else {
        Error(std::string("Unknown shape:") + type);
    }
    set_material_id(shape, material_id);

    // Parse area light emitter
    for (auto child : node.children()) {
        std::string name = child.name();
        if (name == "emitter") {
            Vector3 radiance{1, 1, 1};
            for (auto grand_child : child.children()) {
                std::string name = grand_child.attribute("name").value();
                if (name == "radiance") {
                    radiance = parse_intensity(grand_child, default_map);
                }
            }
            set_area_light_id(shape, lights.size());
            lights.push_back(
                ParsedDiffuseAreaLight{(int)shapes.size() /* shape ID */, radiance});
        }
    }

    return shape;
}

ParsedScene parse_scene(pugi::xml_node node) {
    ParsedCamera camera{
        Vector3{0, 0,  0},
        Vector3{0, 0, -1},
        Vector3{0, 1,  0},
        c_default_fov, // FOV
        c_default_res, c_default_res // width/height
    };
    std::vector<ParsedMaterial> materials;
    std::vector<ParsedLight> lights;
    std::vector<ParsedShape> shapes;
    std::string filename = "image.exr";
    // For <default> tags
    // e.g., <default name="spp" value="4096"/> will map "spp" to "4096"
    std::map<std::string, std::string> default_map;
    std::map<std::string /* name id */, ParsedColor> texture_map;
    std::map<std::string /* name id */, int /* index id */> material_map;
    Vector3 background_color = Vector3{0.5, 0.5, 0.5};
    int sample_count = 16;

    for (auto child : node.children()) {
        std::string name = child.name();
        if (name == "default") {
            parse_default_map(child, default_map);
        } else if (name == "sensor") {
            std::tie(camera, filename, sample_count) =
                parse_sensor(child, default_map);
        } else if (name == "bsdf") {
            std::string material_name;
            ParsedMaterial m;
            std::tie(material_name, m) = parse_bsdf(
                child, texture_map, default_map);
            if (!material_name.empty()) {
                material_map[material_name] = materials.size();
                materials.push_back(m);
            }
        } else if (name == "emitter") {
            lights.push_back(parse_emitter(child, default_map));
        } else if (name == "shape") {
            shapes.push_back(
                parse_shape(child,
                            materials,
                            material_map,
                            texture_map,
                            lights,
                            shapes,
                            default_map));
        } else if (name == "texture") {
            std::string id = child.attribute("id").value();
            if (texture_map.find(id) != texture_map.end()) {
                Error(std::string("Duplicated texture ID:") + id);
            }
            texture_map[id] = parse_texture(child, default_map);
        } else if (name == "background") {
            for (auto grandchild : child.children()) {
                std::string name = grandchild.attribute("name").value();
                if (name == "radiance") {
                    background_color = parse_intensity(grandchild, default_map);
                }
            }
        }
    }
    return ParsedScene{camera,
                       materials,
                       lights,
                       shapes,
                       background_color,
                       sample_count};
}

ParsedScene parse_scene(const fs::path &filename) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
    if (!result) {
        std::cerr << "Error description: " << result.description() << std::endl;
        std::cerr << "Error offset: " << result.offset << std::endl;
        Error("Parse error");
    }
    // back up the current working directory and switch to the parent folder of the file
    fs::path old_path = fs::current_path();
    fs::current_path(filename.parent_path());
    ParsedScene scene = parse_scene(doc.child("scene"));
    // switch back to the old current working directory
    fs::current_path(old_path);
    return scene;
}
