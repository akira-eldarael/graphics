#pragma once
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include "ModelInstance.h"

// Movement speeds
const float CAMERA_SPEED = 3.0f;
const float EDIT_SPEED = 2.0f;
const float ROTATION_SPEED = 5.0f;
const float SCALE_SPEED = 0.1f;

//extern variables to track key states
extern glm::vec3 g_cameraEye;
extern std::vector<ModelInstance> g_modelInstances;
extern size_t g_selectedModelIndex;


//editmode variables
enum EditMode { EDIT_POSITION, EDIT_ORIENTATION, EDIT_SCALE };
extern EditMode g_currentEditMode;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
