#include "ModelInstance.h"
#include <vector>
#pragma once

extern std::vector<ModelInstance> g_modelInstances;
extern size_t g_selectedModelIndex;

bool SaveSceneToFile(const std::string& filename);
bool LoadSceneFromFile(const std::string& filename);

bool FileExists(const std::string& filename);
void CreateDefaultScene();