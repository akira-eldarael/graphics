// DebugSphereGenerator.cpp
#include "DebugSphereGenerator.h"
#include <cmath>
#include <iostream>
#include <glad/glad.h>

bool DebugSphereGenerator::CreateSphere(sModelDrawInfo& drawInfo, unsigned int shaderProgramID,
    float radius, int segments, int rings)
{
    std::vector<sVert> vertices;
    std::vector<unsigned int> indices;

    GenerateSphereGeometry(vertices, indices, radius, segments, rings);

    if (vertices.empty() || indices.empty())
    {
        std::cout << "Failed to generate sphere geometry" << std::endl;
        return false;
    }

    // Set up drawInfo
    drawInfo.meshName = "debug_sphere";
    drawInfo.numberOfVertices = vertices.size();
    drawInfo.numberOfIndices = indices.size();
    drawInfo.numberOfTriangles = indices.size() / 3;

    // Allocate memory for vertices and indices
    drawInfo.pVertices = new sVert[drawInfo.numberOfVertices];
    drawInfo.pIndices = new unsigned int[drawInfo.numberOfIndices];

    // Copy data
    for (size_t i = 0; i < vertices.size(); i++)
    {
        drawInfo.pVertices[i] = vertices[i];
    }

    for (size_t i = 0; i < indices.size(); i++)
    {
        drawInfo.pIndices[i] = indices[i];
    }

    // Create VAO and buffers
    glGenVertexArrays(1, &(drawInfo.VAO_ID));
    glBindVertexArray(drawInfo.VAO_ID);

    // Vertex buffer
    glGenBuffers(1, &(drawInfo.VertexBufferID));
    glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sVert) * drawInfo.numberOfVertices,
        (GLvoid*)drawInfo.pVertices,
        GL_STATIC_DRAW);

    // Index buffer
    glGenBuffers(1, &(drawInfo.IndexBufferID));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * drawInfo.numberOfIndices,
        (GLvoid*)drawInfo.pIndices,
        GL_STATIC_DRAW);

    // Set vertex attributes
    GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPos");
    GLint vcol_location = glGetAttribLocation(shaderProgramID, "vCol");
    GLint vnorm_location = glGetAttribLocation(shaderProgramID, "vNorm");

    if (vpos_location >= 0)
    {
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(sVert), (void*)offsetof(sVert, x));
    }

    if (vnorm_location >= 0)
    {
        glEnableVertexAttribArray(vnorm_location);
        glVertexAttribPointer(vnorm_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(sVert), (void*)offsetof(sVert, nx));
    }

    if (vcol_location >= 0)
    {
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 4, GL_FLOAT, GL_FALSE,
            sizeof(sVert), (void*)offsetof(sVert, r));
    }

    // Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (vpos_location >= 0) glDisableVertexAttribArray(vpos_location);
    if (vcol_location >= 0) glDisableVertexAttribArray(vcol_location);
    if (vnorm_location >= 0) glDisableVertexAttribArray(vnorm_location);

    std::cout << "Created debug sphere with " << drawInfo.numberOfVertices
        << " vertices and " << drawInfo.numberOfTriangles << " triangles" << std::endl;

    return true;
}

void DebugSphereGenerator::GenerateSphereGeometry(std::vector<sVert>& vertices,
    std::vector<unsigned int>& indices,
    float radius, int segments, int rings)
{
    vertices.clear();
    indices.clear();

    const float PI = 3.14159265359f;

    // Generate vertices
    for (int ring = 0; ring <= rings; ring++)
    {
        float phi = PI * float(ring) / float(rings);
        float y = radius * cos(phi);
        float ringRadius = radius * sin(phi);

        for (int segment = 0; segment <= segments; segment++)
        {
            float theta = 2.0f * PI * float(segment) / float(segments);
            float x = ringRadius * cos(theta);
            float z = ringRadius * sin(theta);

            sVert vertex;
            vertex.x = x;
            vertex.y = y;
            vertex.z = z;
            vertex.w = 1.0f;

            // Normal (for a sphere, normal = normalized position)
            glm::vec3 normal = glm::normalize(glm::vec3(x, y, z));
            vertex.nx = normal.x;
            vertex.ny = normal.y;
            vertex.nz = normal.z;
            vertex.nw = 0.0f;

            // Default white color
            vertex.r = 1.0f;
            vertex.g = 1.0f;
            vertex.b = 1.0f;
            vertex.a = 1.0f;

            vertices.push_back(vertex);
        }
    }

    // Generate indices for triangles
    for (int ring = 0; ring < rings; ring++)
    {
        for (int segment = 0; segment < segments; segment++)
        {
            int current = ring * (segments + 1) + segment;
            int next = current + segments + 1;

            // First triangle
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);

            // Second triangle
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
}

//// Usage example function that can be added to main.cpp
//void CreateDebugSphereForCheckpoint()
//{
//    sModelDrawInfo sphereInfo;
//    if (DebugSphereGenerator::CreateSphere(sphereInfo,
//        g_pTheShaderManager->getIDFromFriendlyName("shader1"),
//        1.0f, 16, 12))
//    {
//        // Manually add to VAO manager's internal map if needed
//        // This would require making the map public or adding a method
//        std::cout << "Debug sphere created successfully for checkpoint" << std::endl;
//    }
//    else
//    {
//        std::cout << "Warning: Could not create debug sphere" << std::endl;
//    }
//}