#pragma once
#include <glad/glad.h>
#include "cVAOManager/cVAOManager.h"
#include <string>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\glm.hpp>
#include <vector>

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

extern cVAOManager* g_pMeshManager;
void draw_mesh(ModelInstance instance, GLint program, float transparency);

inline bool extractFolderName(const std::string& input) {
    std::vector<std::string> keywords = { "mig29", "s21", "su47" };
    for (const std::string& word : keywords) {
        if (input.find(word) != std::string::npos) {
            return true;
        }
    }
    return false; // if no keyword is found
}