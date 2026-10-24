#pragma once

#include <glad/glad.h>

// loading textures
unsigned int loadTexture(char const* path, GLint wrapingMode = GL_REPEAT, GLint mipmapMinMode = GL_LINEAR_MIPMAP_LINEAR, GLint mipmapMagMode = GL_LINEAR, bool useSRGB = false);

void flipTexturesOnLoad(bool state);
