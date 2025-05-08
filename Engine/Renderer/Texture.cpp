#include "Texture.hpp"

#include "FileSystem/FileSystem.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
	Texture::Texture(
		u8* data,
		int width,
		int height,
		int channels,
		u32 internalFormat,
		u32 dataFormat)
	{
		m_width = width;
		m_height = height;
		m_channels = channels;
		
		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
		glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_rendererID);
	}

	Texture::Texture(Texture&& other) noexcept
	{
		m_width = other.m_width;
		m_height = other.m_height;
		m_channels = other.m_channels;
		m_internalFormat = other.m_internalFormat;
		m_dataFormat = other.m_dataFormat;
		m_rendererID = other.m_rendererID;

		other.m_rendererID = 0;
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			// If initialized, release the texture from the GPU
			if (m_rendererID)
			{
				glDeleteTextures(1, &m_rendererID);
			}
			
			m_width = other.m_width;
			m_height = other.m_height;
			m_channels = other.m_channels;
			m_internalFormat = other.m_internalFormat;
			m_dataFormat = other.m_dataFormat;
			m_rendererID = other.m_rendererID;
			
			other.m_rendererID = 0;
		}

		return *this;
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


