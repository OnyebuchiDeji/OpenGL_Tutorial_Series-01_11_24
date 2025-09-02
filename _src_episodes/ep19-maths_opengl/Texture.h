#pragma once


#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	//	BPP - bits per pixel
	int m_Width, m_Height, m_BPP;

public:
	Texture(const std::string& path);
	~Texture();

	/**
	*	The parameter specifies the slot that one wants
	*	to bind the texture to; this is because in OpenGL
	*	one can bind several Textures at once, each with their own slots.
	*	In modern GPUs, there are about 232 slots; whereas on Mobile, there might be like 8.
	* 
	*	Though, one can prmpt OpenGL to give how many texture slots one has.
	*/
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline int GetBPP() const { return m_BPP; }
};