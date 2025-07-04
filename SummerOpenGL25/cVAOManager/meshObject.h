#pragma once

#include <string>
#include <glm\glm.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <vector>

class meshObject {
    meshObject();
    std::string meshFileName;
    std::string modelName;
    glm::vec3 position;
    glm::vec3 orientation; //rotation
    glm::vec3 scale;
    glm::vec3 color;

    bool isWireFrame;
    bool useRandomColors;
    bool useRainbowEffect;
    bool isVisible;

    std::vector<meshObject*>child_meshObjs;
};