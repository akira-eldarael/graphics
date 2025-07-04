// DebugSphereGenerator.h
#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "cVAOManager/cVAOManager.h"

class DebugSphereGenerator
{
public:
    static bool CreateSphere(sModelDrawInfo& drawInfo, unsigned int shaderProgramID,
        float radius = 1.0f, int segments = 16, int rings = 12);

private:
    static void GenerateSphereGeometry(std::vector<sVert>& vertices,
        std::vector<unsigned int>& indices,
        float radius, int segments, int rings);
};
