#pragma once



class IndexBuffer
{
private:
	//	This is the numberic ID that is relevant to OpenGL; it basically is the ID that identifies the OpenGL renderer
	//	In an actul Game Engine, there can be, for example, a Vulkan Renderer. So a Vertex Buffer that is used by OpenGL can be made
	//	and one that is used by Vulkan can be made
	unsigned int m_Renderer_ID;
	unsigned int m_Count;

public:
	/*
		Here, 32-bit (unsigned int) indices are supported
		Normally, for small 3d models, using 16-bit indices, unsigned shorts would cause performance increase
		But just to keep it simple, normal unsigned int are used.

		count is how many indices have been supplied.
	*/
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	//	Getter
	inline unsigned int GetCount() const { return m_Count; };
};