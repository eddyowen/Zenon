#include "Texture.h"

#include "Core/Log.h"
#include "FileSystem/FileSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Assert.h"

namespace zn
{
	Texture::Texture(
		unsigned char* data,
		int width,
		int height,
		int channels,
		unsigned int internalFormat,
		unsigned int dataFormat)
	{
		m_width = width;
		m_height = height;
		m_channels = channels;
		
		m_data = data;
		
		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, m_data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	void Texture::Bind(uint32_t textureUnit) const
	{
		glBindTextureUnit(textureUnit, m_rendererID);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}


