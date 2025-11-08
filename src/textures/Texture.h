#pragma once
#include "top_level_include.h"
#include <vector>
unsigned int loadTexture(const char *texturePath);
GLuint loadTextureArray(const std::vector<const char *> texturePaths);
