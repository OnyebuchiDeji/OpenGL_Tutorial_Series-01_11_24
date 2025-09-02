#pragma once


#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


//  This just retrieves and clears all error flags
void GLClearError();

bool GLLogCall(const char* functionName, const char* fileName, int line);