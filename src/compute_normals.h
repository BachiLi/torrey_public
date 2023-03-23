#pragma once

#include "torrey.h"
#include "vector.h"
#include <vector>

/// Given a pool of vertices and a triangle mesh defined on it,
/// compute a set of normals per vertex using weighted average of
/// face normals.
/// In particular, we implemented Nelson Max's method, c.f.,
/// "Computing Vertex Normals from Facet Normals", 1999
std::vector<Vector3> compute_normals(const std::vector<Vector3> &vertices,
                                     const std::vector<Vector3i> &indices);
