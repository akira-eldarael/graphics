// Save scene function
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include "LoadAndSave.h"

bool SaveSceneToFile(const std::string& filename)
{
    try {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Could not open file for writing: " << filename << std::endl;
            return false;
        }

        for (const auto& instance : g_modelInstances)
        {
            file << instance.meshName << " "
                << instance.position.x << " " << instance.position.y << " " << instance.position.z << " "
                << instance.orientation.x << " " << instance.orientation.y << " " << instance.orientation.z << " "
                << instance.scale.x << " " << instance.scale.y << " " << instance.scale.z << "\n";
        }

        file.close();
        std::cout << "Scene saved to " << filename << " with " << g_modelInstances.size() << " models" << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving scene: " << e.what() << std::endl;
        return false;
    }
}


// Load scene function
bool LoadSceneFromFile(const std::string& filename)
{
    try {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "Could not open scene file: " << filename << std::endl;
            return false;
        }

        g_modelInstances.clear();
        std::string line;
        char del;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            ModelInstance instance;

           /* ss >> instance.meshName >>
                instance.position.x >> instance.position.y >> instance.position.z >>
                instance.orientation.x >> instance.orientation.y >> instance.orientation.z >>
                instance.scale.x >> instance.scale.y >> instance.scale.z;*/
            std::getline(ss, instance.meshName, ',');
            ss >> instance.position.x >> del >> instance.position.y >> del >> instance.position.z >> del;
            ss >> instance.orientation.x >> del >> instance.orientation.y >> del >> instance.orientation.z >> del;
            ss >> instance.scale.x >> del >> instance.scale.y >> del >> instance.scale.z;


            g_modelInstances.push_back(instance);
        }

        file.close();
        std::cout << "Loaded " << g_modelInstances.size() << " models from " << filename << std::endl;
        g_selectedModelIndex = 0;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error loading scene: " << e.what() << std::endl;
        return false;
    }
}
// Check if file exists
bool FileExists(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}
// Create default scene with 10 models (using available model files)
void CreateDefaultScene()
{
    try {
        std::cout << "Creating default scene with 10 models..." << std::endl;
        g_modelInstances.clear();

        // Available model files
        std::vector<std::string> availableModels = {
             "assets/dungeon_models/cp_models/Dragon 2.5_ply.ply",
             "assets/dungeon_models/cp_models/Dragon 2.5_ply.ply",
             "assets/dungeon_models/cp_models/Dragon 2.5_ply.ply",
             "assets/dungeon_models/cp_models/Dragon 2.5_ply.ply",
        };

        // Find at least one working model file
        //std::string workingModel = "";
        /*for (const auto& model : availableModels)
        {
            if (FileExists(model))
            {
                workingModel = model;
                break;
            }
        }

        if (workingModel.empty())
        {
            std::cout << "ERROR: No model files found!" << std::endl;
            return;
        }*/

        // Create 10 model instances with different colors and properties
        std::vector<glm::vec3> colors = {
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f), // 
            //glm::vec3(1.0f, 1.0f, 1.0f) // 

            glm::vec3(1.0f, 0.0f, 0.0f), // Red
            glm::vec3(0.0f, 1.0f, 0.0f), // Green
            glm::vec3(0.0f, 0.0f, 1.0f), // Blue
            glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
            glm::vec3(1.0f, 0.0f, 1.0f), // Magenta
            glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
            glm::vec3(1.0f, 0.5f, 0.0f), // Orange
            glm::vec3(0.5f, 0.0f, 1.0f), // Purple
            glm::vec3(1.0f, 1.0f, 1.0f), // White
            glm::vec3(0.5f, 0.5f, 0.5f)  // Gray
        };

        for (int i = 0; i < availableModels.size(); i++)
        {
            ModelInstance instance;

            instance.meshName = availableModels[i % availableModels.size()];
            // Extract display name
            instance.displayName = instance.meshName.substr(instance.meshName.find_last_of("/\\") + 1);
            if (instance.displayName.find('.') != std::string::npos)
            {
                instance.displayName = instance.displayName.substr(0, instance.displayName.find('.'));
            }
            instance.displayName += std::to_string(i + 1); // Add number for uniqueness

            // Arrange in a 5x2 grid pattern
            float x = i * 25.0f - 37.5f;  // Increase from 15 to 25
            float z = i * 10.0f - 20.0f;  // Increase from 5 to 10
            instance.position = glm::vec3(x, 0.0f, z);

            // Different orientations
            instance.orientation = glm::vec3(0.0f, i * 36.0f, 0.0f); // Each rotated differently

            //// Set appropriate scales based on model type
            //if (instance.displayName.find("Teapot") != std::string::npos ||
            //    instance.displayName.find("teapot") != std::string::npos)
            //{
            //    instance.scale = glm::vec3(0.2f + i * 0.02f, 0.2f + i * 0.02f, 0.2f + i * 0.02f);
            //}
            //else if (instance.displayName.find("dolphin") != std::string::npos)
            //{
            //    instance.scale = glm::vec3(0.08f + i * 0.01f, 0.08f + i * 0.01f, 0.08f + i * 0.01f);
            //}
            //else if (instance.displayName.find("cow") != std::string::npos)
            //{
            //    instance.scale = glm::vec3(1.5f + i * 0.2f, 1.5f + i * 0.2f, 1.5f + i * 0.2f);
            //}
            //else if (instance.displayName.find("bun") != std::string::npos)
            //{
            //    instance.scale = glm::vec3(60.0f + i * 5.0f, 60.0f + i * 5.0f, 60.0f + i * 5.0f);
            //}
            //else
            //{
            //    instance.scale = glm::vec3(0.5f + i * 0.1f, 0.5f + i * 0.1f, 0.5f + i * 0.1f);
            //}
            instance.scale = glm::vec3(1.0f);

            // Assign unique colors
            instance.color = colors[i];
            instance.useRandomColors = false;
            instance.useRainbowEffect = false;

            g_modelInstances.push_back(instance);
            std::cout << "Added model " << (i + 1) << ": " << instance.displayName
                << " (Color: " << instance.color.r << ", " << instance.color.g << ", " << instance.color.b << ")" << std::endl;
        }

        std::cout << "Created default scene with " << g_modelInstances.size() << " models" << std::endl;

        //Save the scene
        //SaveSceneToFile("scene.txt");
    }
    catch (const std::exception& e) {
        std::cout << "Error creating default scene: " << e.what() << std::endl;
    }
}