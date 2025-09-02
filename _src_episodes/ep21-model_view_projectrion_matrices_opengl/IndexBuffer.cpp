#include "IndexBuffer.h"

#include "Renderer.h"



IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    GLCall(glGenBuffers(1, &m_Renderer_ID));
    //  Specifies what the buffer is used for
    //  binding the buffer shows it's to be used
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
    /**
    *   There may be a danger here, in this: 'count * sizeof(unsigned int)', because there may be a platform where
    *   the size of an unsigned int is not 32 bits. But this is almost rare.
    *   The reason is that the below takes in a GLuint which is 32 bits.
    */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}


IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_Renderer_ID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Renderer_ID));
}


void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}