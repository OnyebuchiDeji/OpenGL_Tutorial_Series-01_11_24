#include "Texture.h"

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	/**
	*	This flips the texture vertically because OpenGL expects texture pixles to start at
	*	the bottom left at (0, 0) for OpenGL not top left.
	*	
	*	Now, typically when reading an image, it's scanned and stored using scan lines from the
	*	top to the bottom of the image.
	*	Though this depends on the texture format. For PNGs, though, this is true.
	*/	
	stbi_set_flip_vertically_on_load(1);
	/**
	*	It will return a pointer to a buffer containing the information of the loaded image
	*	passing the references to m_Width, m_Height, and m_BPP is becasue `stbi_load` will
	*	assign the corresponding values it got from the loaded image into them so they store it:
	*	width, height, bits per pixel 
	*/

	//	Now the Texture data is inside this local buffer
	//	For the last parameter, you could add: STBI_rgb or 4
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//	Now configure settings for the Texture that's just being generated:

	//	setting texture parameters -- THEY MUST BE SET FOR YOUR TEXTURE TO RENDER
	/**
	*	The MIN_FILTER resamples the image down if it needs to be rendered smaller than it originally
	*	is per pixel.
	*	GL_LINEAR: Linearly resamples everything so the render does not snap to the nearest pixel, which is what
	*	GL_NEAREST would do.
	* 
	*	Then the second specifies the Magnification Filter
	* 
	*	Then the third specifies the clamp modes or wrap modes:	
	*		Wrap_S (horizontal wraps) : clamp or not extend the area
	*		Wrap_T (vertical wraps)
	*	It's these vs. tileing the texture, which is not what we want
	* 
	*	If these parameters are not specified, a Black Texture is gotten. There are no default
	*	settings -- gg OpenGL.
	* 
	*/
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//	Now give OpenGL the image pixel data.
	/**
	*	p1: gl target -- GL_TEXTURE_2D
	*	p2:	texture_level -- defines if it's a multi-level texture or not
	*	p3: internal_format -- how OpenGL should read/see/interpret the pixel data format.	It's not the format
	*		of the data being supplied -- should be GL_RGBA8 in recent implementations
	*		as the 8 specifies the number of bits per channel
	*	p4: width, p5: height
	*	p6:	format of the image data being supplied 
	*	p7:	Type of data, which is 8 bytes, unsigned
	*	p8:	pointer to pixel buffer -- now for this, one can specify a nullptr if not ready to supply data, which allocates
	*	space on the GPU, as it knows how much space it needs, without providing the actual data to work with.
	*/
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//	Unbind the texture
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	/**
	*	Now, at times rather than freeing this data, one may want to leave it on the CPU
	*	in case one wants to sample it, for example.
	*/
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	/**
	*	Specify the textue slot.The below takes in an integer that specifies which
	*	texture slot one is currently activating.
	*	This means any texture that is bound after activating, e.g. slot 16, will always bind to this slot
	*	until one activates another slot by specifying a different slot number.
	* 
	*	The GL_TEXTURE0 is the very first slot, there's up to GL_TEXTURE31, so 32 slots from 0-31.
	*	These enums are just integers.
	*	Now, 32 slots is the maximum number of texture slots tha tthe OpenGL specification allows;
	*	it does not mean that one's device/platform supports that many slotss. E.g. Mobile has ~8/.
	*	
	*	They are integers, hence the slots can be added.
	*/

	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
