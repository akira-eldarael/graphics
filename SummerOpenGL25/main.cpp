#define _CRT_SECURE_NO_WARNINGS
    
#include "GlobalOpenGL.h"
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
#include <algorithm>

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
/*test camera positions*/
glm::vec3 g_cameraEye = glm::vec3(-90.0, -474.5, 241.0f);

std::vector<ModelInstance> g_modelInstances;
size_t g_selectedModelIndex = 0;
std::vector<std::string> availableModels = {
             "assets/dungeon_models/cp_models/Dragon2.5_ply.ply",
             "assets/dungeon_models/cp_models/mig29.ply",
             "assets/dungeon_models/cp_models/s21_No_normals.ply",
             "assets/dungeon_models/cp_models/Seafloor2.ply",
             "assets/dungeon_models/cp_models/SM_Env_Crystals_Cluster_Large_01.ply",
             "assets/dungeon_models/cp_models/SM_Env_Crystals_Cluster_Large_02.ply",
             "assets/dungeon_models/cp_models/ssj100.ply",
             "assets/dungeon_models/cp_models/su47.ply",
};
void InitializeModels()
{
    //LoadSceneFromFile("assets/dungeon_models/cp_models/transforms.csv");
    LoadSceneFromFile("assets/dungeon_models/cp_models/transforms.csv");
    //CreateDefaultScene();
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

        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearDepth(1.0);
        

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
        g_pTheLightManager->theLights[0].position = glm::vec4(-96.0896f, -9.87097f, 430.0f, 1.0f);
		g_pTheLightManager->theLights[0].diffuse = glm::vec4(1.0f, 0.94f, 0.87f, 1.0f); // is supposed to be Red turned white for testing
        g_pTheLightManager->theLights[0].specular = glm::vec4(1.0f, 0.94f, 0.87f, 1.0f);
        g_pTheLightManager->theLights[0].atten = glm::vec4(0.1f, 0.0001f, 0.0001f, 1.0f); // Low linear, light quadratic
        g_pTheLightManager->theLights[0].param1.x = 0.0f; // Point light
        g_pTheLightManager->theLights[0].param2.x = 1.0f; // Turn ON

        // Light 1 - Bright Green (top-right area)  
        g_pTheLightManager->theLights[1].position = glm::vec4(141.571f, -15.3884f, 800.0f, 1.0f);
		g_pTheLightManager->theLights[1].diffuse = glm::vec4(1.0f, 0.94f, 0.87f, 1.0f); // is supposed to be Green turned to white for testing
        g_pTheLightManager->theLights[1].specular = glm::vec4(1.0f, 0.94f, 0.87f, 1.0f);
        g_pTheLightManager->theLights[1].atten = glm::vec4(0.1f, 0.0001f, 0.00001f, 1.0f);
        g_pTheLightManager->theLights[1].param1.x = 0.0f; // Point light
        g_pTheLightManager->theLights[1].param2.x = 1.0f; // Turn ON

        // Light 2 - Bright Blue (center-back area)
        g_pTheLightManager->theLights[2].position = glm::vec4(-200.549, -40.779, 500.503624, 1.0f);
		g_pTheLightManager->theLights[2].diffuse = glm::vec4(1.0f, 0.94f, 0.87f, 1.0f); // is supposed to be Blue turnde to white for test
        g_pTheLightManager->theLights[2].specular = glm::vec4(1.0f, 0.94f, 0.87f, 1.0f);
        g_pTheLightManager->theLights[2].atten = glm::vec4(0.1f, 0.0001f, 0.00001f, 1.0f);
        g_pTheLightManager->theLights[2].param1.x = 0.0f; // Point light
        g_pTheLightManager->theLights[2].param2.x = 1.0f; // Turn ON

        std::cout << "3 colored lights initialized: Red, Green, Blue" << std::endl;

		g_pMeshManager = new cVAOManager();

        // Initialize models
        InitializeModels();
        /*test space*/
        // Load models into VAO
        for (size_t i = 0; i < availableModels.size(); i++)
        {
            std::string _name = availableModels[i];

            sModelDrawInfo modelInfo;
            if (g_pMeshManager->LoadModelIntoVAO(_name, modelInfo, program, true, true))
            {
                std::cout << "Loaded: " << _name << std::endl;
            }
            else
            {
                std::cout << "Failed to load: " << _name << std::endl;
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
        /*enable transpercy{to let the gpu know we will be using aplha valur from now on}*/
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        /*split for transperant objects*/
        std::vector<ModelInstance> t_objects;
        for (size_t i = 0; i < g_modelInstances.size(); i++)
        {
            if (extractFolderName(g_modelInstances[i].meshName))t_objects.push_back(g_modelInstances[i]);   
        }
        // Main render loop
        while (!glfwWindowShouldClose(window))
        {
            float transperancy = 1.0f;
            try {
                float ratio;
                int width, height;
                glm::mat4 projection, view, mvp;

                glfwGetFramebufferSize(window, &width, &height);
                ratio = width / (float)height;
                glViewport(0, 0, width, height);

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                projection = glm::perspective(glm::radians(45.0f), ratio, 1.0f, 100000.0f);
                glm::vec3 cameraTarget = glm::vec3(-315.25927734375f, -261.2432861328125f, 77.5234146118164f);
                //glm::vec3 cameraTarget = glm::vec3(-307.9396667480469f, 236.92022705078125f, 95.74645233154297f);
                glm::vec3 upVector = glm::vec3(0.0f, 0.0f, 1.0f);
                view = glm::lookAt(g_cameraEye, cameraTarget, upVector);

                glUseProgram(program);
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                /*tittle space*/
                std::ostringstream titleStream;
                titleStream << "camera : (" << g_cameraEye.x << ", " << g_cameraEye.y << ", " << g_cameraEye.z << ")		"
                    /* "target : (" << cameraTarget.x << ", " << cameraTarget.y << ", " << cameraTarget.z << ")"*/;

                glfwSetWindowTitle(window, titleStream.str().c_str());
                // Set up lighting uniforms
                GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
                glUniform3f(eyeLocation_UL, g_cameraEye.x, g_cameraEye.y, g_cameraEye.z);

                // Update all light uniforms
                g_pTheLightManager->UpdateShaderUniforms(program);

                // Get matrix uniform locations
               
                GLint mView_UL = glGetUniformLocation(program, "mView");
                GLint mProj_UL = glGetUniformLocation(program, "mProj");
                
                /*for transparency*/
               

                // Set view and projection matrices (same for all objects)
                glUniformMatrix4fv(mView_UL, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(mProj_UL, 1, GL_FALSE, glm::value_ptr(projection));
                /*sort for T objects*/
                std::sort(t_objects.begin(), t_objects.end(),
                    [](const ModelInstance& a, const ModelInstance& b) {
                        float distA = glm::distance(g_cameraEye, a.position);
                        float distB = glm::distance(g_cameraEye, b.position);
                        return distA > distB; // Sort from farthest to nearest
                    });
                // Render all 10 models
                for (size_t i = 0; i < g_modelInstances.size(); i++) {
                    if (extractFolderName(g_modelInstances[i].meshName)) {
                       /* draw_mesh(g_modelInstances[i], program, transperancy);
                        transperancy -= 0.1428571428571429;*/
                        continue;
                    }
                    else draw_mesh(g_modelInstances[i], program, 1.0f);
                }
                for (auto& ins : t_objects) {
                    transperancy-= 0.1128571428571429;
                    draw_mesh(ins, program, transperancy);
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
