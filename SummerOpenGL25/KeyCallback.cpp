#include "KeyCallBack.h"
#include <iostream>
#include <glm/common.hpp>
#include "LoadAndSave.h"

EditMode g_currentEditMode = EDIT_POSITION;
// Key callback function - SUPPORTS HOLDING KEYS
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    try {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            std::cout << "Escape pressed - closing window" << std::endl;
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            return;
        }

        // Handle both PRESS and REPEAT for holding keys
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            switch (key)
            {
                // Camera controls - holding works
            case GLFW_KEY_A:
                g_cameraEye.x -= CAMERA_SPEED;
                break;
            case GLFW_KEY_D:
                g_cameraEye.x += CAMERA_SPEED;
                break;
            case GLFW_KEY_W:
                g_cameraEye.z -= CAMERA_SPEED;
                break;
            case GLFW_KEY_S:
                g_cameraEye.z += CAMERA_SPEED;
                break;
            case GLFW_KEY_Q:
                g_cameraEye.y += CAMERA_SPEED;
                break;
            case GLFW_KEY_E:
                g_cameraEye.y -= CAMERA_SPEED;
                break;

                // Object editing controls - holding works
            case GLFW_KEY_LEFT:
            case GLFW_KEY_RIGHT:
            case GLFW_KEY_UP:
            case GLFW_KEY_DOWN:
            case GLFW_KEY_PAGE_UP:
            case GLFW_KEY_PAGE_DOWN:
            case GLFW_KEY_EQUAL:
            case GLFW_KEY_MINUS:
                if (!g_modelInstances.empty() && g_selectedModelIndex < g_modelInstances.size())
                {
                    ModelInstance& selected = g_modelInstances[g_selectedModelIndex];

                    switch (g_currentEditMode)
                    {
                    case EDIT_POSITION:
                        switch (key)
                        {
                        case GLFW_KEY_LEFT:
                            selected.position.x -= EDIT_SPEED;
                            break;
                        case GLFW_KEY_RIGHT:
                            selected.position.x += EDIT_SPEED;
                            break;
                        case GLFW_KEY_UP:
                            selected.position.z -= EDIT_SPEED;
                            break;
                        case GLFW_KEY_DOWN:
                            selected.position.z += EDIT_SPEED;
                            break;
                        case GLFW_KEY_PAGE_UP:
                            selected.position.y += EDIT_SPEED;
                            break;
                        case GLFW_KEY_PAGE_DOWN:
                            selected.position.y -= EDIT_SPEED;
                            break;
                        }
                        break;

                    case EDIT_ORIENTATION:
                        switch (key)
                        {
                        case GLFW_KEY_LEFT:
                            selected.orientation.y -= ROTATION_SPEED;
                            break;
                        case GLFW_KEY_RIGHT:
                            selected.orientation.y += ROTATION_SPEED;
                            break;
                        case GLFW_KEY_UP:
                            selected.orientation.x -= ROTATION_SPEED;
                            break;
                        case GLFW_KEY_DOWN:
                            selected.orientation.x += ROTATION_SPEED;
                            break;
                        case GLFW_KEY_PAGE_UP:
                            selected.orientation.z += ROTATION_SPEED;
                            break;
                        case GLFW_KEY_PAGE_DOWN:
                            selected.orientation.z -= ROTATION_SPEED;
                            break;
                        }
                        break;

                    case EDIT_SCALE:
                        switch (key)
                        {
                        case GLFW_KEY_LEFT:
                            selected.scale.x = glm::max(0.01f, selected.scale.x - SCALE_SPEED);
                            break;
                        case GLFW_KEY_RIGHT:
                            selected.scale.x += SCALE_SPEED;
                            break;
                        case GLFW_KEY_UP:
                            selected.scale.z += SCALE_SPEED;
                            break;
                        case GLFW_KEY_DOWN:
                            selected.scale.z = glm::max(0.01f, selected.scale.z - SCALE_SPEED);
                            break;
                        case GLFW_KEY_PAGE_UP:
                            selected.scale.y += SCALE_SPEED;
                            break;
                        case GLFW_KEY_PAGE_DOWN:
                            selected.scale.y = glm::max(0.01f, selected.scale.y - SCALE_SPEED);
                            break;
                        case GLFW_KEY_EQUAL: // + key (uniform scale up)
                            selected.scale += glm::vec3(SCALE_SPEED);
                            break;
                        case GLFW_KEY_MINUS: // - key (uniform scale down)
                            selected.scale = glm::max(glm::vec3(0.01f), selected.scale - glm::vec3(SCALE_SPEED));
                            break;
                        }
                        break;
                    }
                }
                break;
            }
        }

        // Only handle these on PRESS (not repeat)
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
                // Object selection - TAB to select individual meshes
            case GLFW_KEY_TAB:
                std::cout << "TAB pressed!" << std::endl;
                if (!g_modelInstances.empty())
                {
                    size_t oldIndex = g_selectedModelIndex;
                    g_selectedModelIndex = (g_selectedModelIndex + 1) % g_modelInstances.size();
                    std::cout << "Selection changed from " << oldIndex + 1 << " to " << g_selectedModelIndex + 1 << std::endl;
                    std::cout << "Selected: " << g_modelInstances[g_selectedModelIndex].displayName << std::endl;
                }
                else
                {
                    std::cout << "No models to select!" << std::endl;
                }
                break;

                // Edit modes - keyboard controls to update objects
            case GLFW_KEY_1: // Position
                g_currentEditMode = EDIT_POSITION;
                std::cout << "Edit mode: POSITION" << std::endl;
                break;
            case GLFW_KEY_2: // Orientation
                g_currentEditMode = EDIT_ORIENTATION;
                std::cout << "Edit mode: ORIENTATION" << std::endl;
                break;
            case GLFW_KEY_3: // Scale
                g_currentEditMode = EDIT_SCALE;
                std::cout << "Edit mode: SCALE" << std::endl;
                break;

                // File operations - save and load scene
            case GLFW_KEY_F5:
                std::cout << "F5 - Saving scene..." << std::endl;
                SaveSceneToFile("scene.txt");
                break;
            case GLFW_KEY_F9:
                std::cout << "F9 - Loading scene..." << std::endl;
                LoadSceneFromFile("scene.txt");
                break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "ERROR in key_callback: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "UNKNOWN ERROR in key_callback!" << std::endl;
    }
}