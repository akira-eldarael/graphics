#define _CRT_SECURE_NO_WARNINGS

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>

#include"ModelInstance.h"
#include "KeyCallBack.h"
#include "LoadAndSave.h"
#include "cShaderManager/cShaderManager.h"
#include "cVAOManager/cVAOManager.h"
#include "cLightManager.h"


// Global variables
cLightManager* g_pTheLightManager = NULL;
cShaderManager* g_pTheShaderManager = NULL;
cVAOManager* g_pMeshManager = NULL;
glm::vec3 g_cameraEye = glm::vec3(-20.0, 45.5, 101.0f);

std::vector<ModelInstance> g_modelInstances;
size_t g_selectedModelIndex = 0;

void InitializeModels()
{
    //LoadSceneFromFile("scene.txt");
    CreateDefaultScene();
    //if (!LoadSceneFromFile("scene.txt")) { CreateDefaultScene(); }
    
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}



int main(void)
{
    GLFWwindow* window;
    GLuint program;
    GLint mvp_location;

    try {
        glfwSetErrorCallback(error_callback);
        if (!glfwInit())
        {
            std::cout << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(1200, 800, "INFO-3111 Checkpoint #5", NULL, NULL);
        if (!window)
        {
            std::cout << "Failed to create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetKeyCallback(window, key_callback);
        glfwMakeContextCurrent(window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glfwSwapInterval(1);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        

        // Initialize shaders
        g_pTheShaderManager = new cShaderManager();
        g_pTheShaderManager->setBasePath("assets/shaders/");

        cShaderManager::cShader vert_shader;
        vert_shader.fileName = "vertex_shader.glsl";

        cShaderManager::cShader frag_shader;
        frag_shader.fileName = "fragment_shader.glsl";

        if (g_pTheShaderManager->createProgramFromFile("shader1", vert_shader, frag_shader))
        {
            std::cout << "Shaders created successfully!" << std::endl;
        }
        else
        {
            std::cout << "Shader creation failed: " << g_pTheShaderManager->getLastError() << std::endl;
            return -1;
        }

        program = g_pTheShaderManager->getIDFromFriendlyName("shader1");
        mvp_location = glGetUniformLocation(program, "MVP");

		g_pTheLightManager = new cLightManager();
		g_pTheLightManager->GetUniformLocations(program);

        // Set up 3 different colored lights for checkpoint
        // Light 0 - Bright Red (top-left area)
        g_pTheLightManager->theLights[0].position = glm::vec4(-40.0f, 50.0f, 30.0f, 1.0f);
		g_pTheLightManager->theLights[0].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // is supposed to be Red turned white for testing
        g_pTheLightManager->theLights[0].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_pTheLightManager->theLights[0].atten = glm::vec4(0.1f, 0.01f, 0.001f, 1.0f); // Low linear, light quadratic
        g_pTheLightManager->theLights[0].param1.x = 0.0f; // Point light
        g_pTheLightManager->theLights[0].param2.x = 1.0f; // Turn ON

        // Light 1 - Bright Green (top-right area)  
        g_pTheLightManager->theLights[1].position = glm::vec4(40.0f, 50.0f, 30.0f, 1.0f);
		g_pTheLightManager->theLights[1].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // is supposed to be Green turned to white for testing
        g_pTheLightManager->theLights[1].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_pTheLightManager->theLights[1].atten = glm::vec4(0.1f, 0.01f, 0.001f, 1.0f);
        g_pTheLightManager->theLights[1].param1.x = 0.0f; // Point light
        g_pTheLightManager->theLights[1].param2.x = 1.0f; // Turn ON

        // Light 2 - Bright Blue (center-back area)
        g_pTheLightManager->theLights[2].position = glm::vec4(0.0f, 40.0f, -50.0f, 1.0f);
		g_pTheLightManager->theLights[2].diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // is supposed to be Blue turnde to white for test
        g_pTheLightManager->theLights[2].specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_pTheLightManager->theLights[2].atten = glm::vec4(0.1f, 0.01f, 0.001f, 1.0f);
        g_pTheLightManager->theLights[2].param1.x = 0.0f; // Point light
        g_pTheLightManager->theLights[2].param2.x = 1.0f; // Turn ON

        std::cout << "3 colored lights initialized: Red, Green, Blue" << std::endl;

		g_pMeshManager = new cVAOManager();

        // Initialize models
        InitializeModels();

        // Load models into VAO
        for (size_t i = 0; i < g_modelInstances.size(); i++)
        {
            const auto& instance = g_modelInstances[i];
            std::string instanceKey = instance.meshName + "_instance_" + std::to_string(i);

            sModelDrawInfo modelInfo;
            if (g_pMeshManager->LoadModelIntoVAO(instance.meshName, modelInfo, program, true, true))
            {
                std::cout << "Loaded: " << instance.displayName << " (Color: "
                    << instance.color.r << ", " << instance.color.g << ", " << instance.color.b << ")" << std::endl;
            }
            else
            {
                std::cout << "Failed to load: " << instance.meshName << std::endl;
            }
        }

        if (!g_modelInstances.empty())
        {
            std::cout << "\nReady! 10 Models loaded with different colors:" << std::endl;
            for (size_t i = 0; i < g_modelInstances.size(); i++)
            {
                const auto& instance = g_modelInstances[i];
                std::cout << "  " << (i + 1) << ". " << instance.displayName
                    << " - Color(" << instance.color.r << ", " << instance.color.g << ", " << instance.color.b << ")" << std::endl;
            }
            std::cout << "\nSelected: " << g_modelInstances[g_selectedModelIndex].displayName << " (1 of " << g_modelInstances.size() << ")" << std::endl;
        }
        else
        {
            std::cout << "WARNING: No models loaded!" << std::endl;
        }

        // Main render loop
        while (!glfwWindowShouldClose(window))
        {
            try {
                float ratio;
                int width, height;
                glm::mat4 projection, view, mvp;

                glfwGetFramebufferSize(window, &width, &height);
                ratio = width / (float)height;
                glViewport(0, 0, width, height);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 1000.0f);
                glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
                view = glm::lookAt(g_cameraEye, cameraTarget, upVector);

                glUseProgram(program);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                // Set up lighting uniforms
                GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
                glUniform3f(eyeLocation_UL, g_cameraEye.x, g_cameraEye.y, g_cameraEye.z);

                // Update all light uniforms
                g_pTheLightManager->UpdateShaderUniforms(program);

                // Get matrix uniform locations
                GLint mModel_UL = glGetUniformLocation(program, "mModel");
                GLint mView_UL = glGetUniformLocation(program, "mView");
                GLint mProj_UL = glGetUniformLocation(program, "mProj");
                GLint mModel_InverseTranpose_UL = glGetUniformLocation(program, "mModel_InverseTranpose");

                // Set view and projection matrices (same for all objects)
                glUniformMatrix4fv(mView_UL, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(mProj_UL, 1, GL_FALSE, glm::value_ptr(projection));

                // Render all 10 models
                for (size_t i = 0; i < g_modelInstances.size(); i++)
                {
                    const auto& instance = g_modelInstances[i];

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, instance.position);
                    model = glm::rotate(model, glm::radians(instance.orientation.x), glm::vec3(1, 0, 0));
                    model = glm::rotate(model, glm::radians(instance.orientation.y), glm::vec3(0, 1, 0));
                    model = glm::rotate(model, glm::radians(instance.orientation.z), glm::vec3(0, 0, 1));
                    model = glm::scale(model, instance.scale);

                    // Calculate inverse transpose for normal transformation
                    glm::mat4 mModel_InverseTranspose = glm::inverse(glm::transpose(model));

                    // Set model matrix uniforms
                    glUniformMatrix4fv(mModel_UL, 1, GL_FALSE, glm::value_ptr(model));
                    glUniformMatrix4fv(mModel_InverseTranpose_UL, 1, GL_FALSE, glm::value_ptr(mModel_InverseTranspose));

                    mvp = projection * view * model;
                    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

                    sModelDrawInfo modelToDraw;
                    if (g_pMeshManager->FindDrawInfoByModelName(instance.meshName, modelToDraw))
                    {
                        // Highlight selected object with thicker lines
                        if (i == g_selectedModelIndex)
                        {
                            glLineWidth(4.0f); // Extra thick for selected object
                        }
                        else
                        {
                            glLineWidth(1.0f);
                        }

                        glBindVertexArray(modelToDraw.VAO_ID);
                        glDrawElements(GL_TRIANGLES, modelToDraw.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
                        glBindVertexArray(0);
                    }
                }

                glLineWidth(1.0f); // Reset line width
                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            catch (const std::exception& e) {
                std::cout << "Error in render loop: " << e.what() << std::endl;
            }
        }

        // Save scene before exit
        //std::cout << "Saving scene before exit..." << std::endl;
        //SaveSceneToFile("scene.txt");

        // Cleanup
        delete g_pMeshManager;
        delete g_pTheShaderManager;
		delete g_pTheLightManager;

        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }
    catch (const std::exception& e) {
        std::cout << "FATAL ERROR: " << e.what() << std::endl;
        return -1;
    }
}

/*
* without apply uniform color the model will be loaded with half colored becuase the model is not colored inside the mesh {true}
* the new color is not applied even though the model instance is updated with then new color becuase
* after changing the model instance color the we need to update the vertices of all the models before uploading to openGL
* if u can use the mesh with completly colored model then u can skip this function
* but u need to update the LoadModelIntoVao accordingly
* 
            for (unsigned int i = 0; i < drawInfo.numberOfVertices; i++)
            {
                drawInfo.pVertices[i].r = color.r;
                drawInfo.pVertices[i].g = color.g;
                drawInfo.pVertices[i].b = color.b;
                modelInfo.pVertices[j].a = 1.0f;//if its rgba
            }
*Update vertex buffer with new colors
            glBindBuffer(GL_ARRAY_BUFFER, modelInfo.VertexBufferID);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sVert) * modelInfo.numberOfVertices, modelInfo.pVertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
     
*/
