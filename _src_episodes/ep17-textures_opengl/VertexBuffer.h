#pragma once



class VertexBuffer
{
private:
	//	This is the numberic ID that is relevant to OpenGL; it basically is the ID that identifies the OpenGL renderer
	//	In an actul Game Engine, there can be, for example, a Vulkan Renderer. So a Vertex Buffer that is used by OpenGL can be made
	//	and one that is used by Vulkan can be made
	unsigned int m_Renderer_ID;

public:
	//	Size is bytes that the vertices specified occupy.
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};