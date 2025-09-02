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

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
{
    shader.Bind();
    //  Bind just the Vertex Array Object
    vao.Bind();
    //  bind index buffer
    ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr)); //  <- used with an index buffer.

}


