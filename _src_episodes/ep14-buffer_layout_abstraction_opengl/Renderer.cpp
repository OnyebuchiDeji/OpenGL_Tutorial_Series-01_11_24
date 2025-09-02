#pragma once


#include "Renderer.h"
#include <iostream>



//  This just retrieves and clears all error flags
void GLClearError()
{
    //  Can be written as `while (glGetError() != GL_NO_ERROR);`
    //  It keeps retrieving errors and clearing the flags until there are no more errors
    while (glGetError());
}

bool GLLogCall(const char* functionName, const char* fileName, int line)
{
    //  The below checks for an error and since it returns a GLenum (same as an unsigned int enum)
    //  this assignes the value to the variable 'error'.
    //  It gets the error as long as the 'error' value gotten is not 0
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << functionName << " " << fileName << ":" << line << "\n";
        return false;
    }
    return true;
}
