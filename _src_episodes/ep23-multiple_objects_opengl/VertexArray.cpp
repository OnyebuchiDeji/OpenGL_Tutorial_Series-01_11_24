#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
	//(*this).Bind(); or this->Bind()
	// or just
	Bind();
	//	Bind the VBO
	vbo.Bind();

	/*Setup The Layout*/
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i=0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		//  this can be called anywhere; it enables this buffer to be used
		GLCall(glEnableVertexAttribArray(i));
		//  The data for the positions
		//  @Exp: Notice how the layout of this Vertex Buffer is specified here
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

		//	At this point, th esize of each type is needed.
		offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}