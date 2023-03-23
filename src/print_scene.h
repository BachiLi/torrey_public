#pragma once

#include "parse_scene.h"
#include <iostream>

std::ostream& operator<<(std::ostream &os, const ParsedColor &color);
std::ostream& operator<<(std::ostream &os, const ParsedCamera &camera);
std::ostream& operator<<(std::ostream &os, const ParsedMaterial &material);
std::ostream& operator<<(std::ostream &os, const ParsedLight &light);
std::ostream& operator<<(std::ostream &os, const ParsedShape &shape);
std::ostream& operator<<(std::ostream &os, const ParsedScene &scene);
