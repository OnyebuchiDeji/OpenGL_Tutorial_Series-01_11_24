#pragma once


#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


//  This just retrieves and clears all error flags
void GLClearError();

bool GLLogCall(const char* functionName, const char* fileName, int line);

/**
*   Some people choose to make this a Singleton, and some don''t because they would
*   want multiple instances of the Renderer.
* 
*   Here, we make it non-Singleton, so it's possible to have multiple of this.
*/
class Renderer
{
public:
    /**
    *   Needs Vertex Array(which has Vertex Buffer bound to it), Index Buffer, Valid Shader.
    *   Now, the index buffer has an Index Count -- it can be drawn partially, or drawn considering the whole index buffer.
    *   But here, because no Index Count is being passed (the Index Count determines which the number of indices in the buffer
    *   to consider), to draw with a partial Index Buffer, an Index Buffer with a partial set of indices will be passed.
    */
    void Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};