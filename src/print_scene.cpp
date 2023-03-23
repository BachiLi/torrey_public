#include "print_scene.h"

std::ostream& operator<<(std::ostream &os, const ParsedColor &color) {
    os << "Color[type=";
    if (auto *rgb = std::get_if<Vector3>(&color)) {
        os << "RGB, value=" << (*rgb) << "]";
    } else if (auto *texture = std::get_if<ParsedImageTexture>(&color)) {
        os << "ImageTexture, filename=" << texture->filename << ", " <<
                            "uscale=" << texture->uscale << ", " <<
                            "vscale=" << texture->vscale << ", " <<
                            "uoffset=" << texture->uscale << ", " <<
                            "voffset=" << texture->vscale << "]";
    } else {
        // Likely an unhandled case.
        os << "Unknown]";
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const ParsedCamera &camera) {
    os << "Camera[lookfrom=" << camera.lookfrom << 
               ", lookat=" << camera.lookat << 
               ", up=" << camera.up <<
               ", vfov=" << camera.vfov <<
               ", width=" << camera.width <<
               ", height=" << camera.height << "]";
    return os;
}

std::ostream& operator<<(std::ostream &os, const ParsedMaterial &material) {
    os << "Material[type=";
    if (auto *diffuse = std::get_if<ParsedDiffuse>(&material)) {
        os << "Diffuse, reflectance=" << diffuse->reflectance << "]";
    } else if (auto *mirror = std::get_if<ParsedMirror>(&material)) {
        os << "Mirror, reflectance=" << mirror->reflectance << "]";
    } else {
        // Likely an unhandled case.
        os << "Unknown]";
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const ParsedLight &light) {
    os << "Light[type=";
    if (auto *point = std::get_if<ParsedPointLight>(&light)) {
        os << "Point, position=" << point->position << ", intensity=" << point->intensity << "]";
    } else if (auto *diffuse_area_light = std::get_if<ParsedDiffuseAreaLight>(&light)) {
        os << "DiffuseAreaLight, shape_id=" << 
            diffuse_area_light->shape_id << 
            ", radiance=" << diffuse_area_light->radiance << "]";
    } else {
        // Likely an unhandled case.
        os << "Unknown]";
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const ParsedShape &shape) {
    os << "Shape[type=";
    if (auto *sphere = std::get_if<ParsedSphere>(&shape)) {
        os << "Sphere, " << 
              "material_id=" << sphere->material_id << ", " <<
              "area_light_id=" << sphere->area_light_id << ", " <<
              "position=" << sphere->position << ", " <<
              "radius=" << sphere->radius << "]";
    } else if (auto *triangle_mesh = std::get_if<ParsedTriangleMesh>(&shape)) {
        os << "TriangleMesh, " << 
              "material_id=" << triangle_mesh->material_id << ", " << 
              "area_light_id=" << triangle_mesh->area_light_id << ", " <<
              "num_vertices=" << triangle_mesh->positions.size() << ", " <<
              "num_faces=" << triangle_mesh->indices.size() << ", " <<
              "has_normals=" << (triangle_mesh->normals.size() > 0) << ", " <<
              "has_uvs=" << (triangle_mesh->uvs.size() > 0) << "]";
    } else {
        // Likely an unhandled case.
        os << "Unknown]";
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const ParsedScene &scene) {
    os << "Scene[";
    os << scene.camera << std::endl;
    for (auto m : scene.materials) {
        os << "\t" << m << std::endl;
    }
    for (auto l : scene.lights) {
        os << "\t" << l << std::endl;
    }
    for (auto s : scene.shapes) {
        os << "\t" << s << std::endl;
    }
    os << "\tsamples_per_pixel=" << scene.samples_per_pixel << "]";
    return os;
}
