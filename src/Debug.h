#pragma once

#include <glad/glad.h>

// setup openGL debugging
void setupOpenGLDebugOutput();

// debug context output
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

// error handling - simple version, must be called directly by the user
// call glCheckError() as defined in Debug.h
GLenum glCheckError_(const char* file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__) 