#pragma once

#include "torrey.h"
#include "matrix.h"
#include "parse_scene.h"
#include <filesystem>

/// Parse Mitsuba's serialized file format.
ParsedTriangleMesh parse_serialized(const fs::path &filename,
                                    int shape_index,
                                    const Matrix4x4 &to_world);
