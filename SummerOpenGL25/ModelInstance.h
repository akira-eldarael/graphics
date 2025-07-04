#pragma once
#include <string>
#include <glm/ext/vector_float3.hpp>
// Model instance structure
struct ModelInstance
{
    std::string meshName;
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 scale;
    glm::vec3 color;
    bool useRandomColors;
    bool useRainbowEffect;
    std::string displayName;
};
