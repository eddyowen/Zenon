#pragma once

#include "Core/Base.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
	class Texture
	{
	public:
		Texture(
			u8* data,
			int width,
			int height,
			int channels,
			u32 internalFormat,
			u32 dataFormat);
		
		~Texture();

		Texture(const Texture& other)
		: m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels),
		  m_internalFormat(other.m_internalFormat), m_dataFormat(other.m_dataFormat)
			{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
			glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

			// If you want to copy the pixel data from GPU, you'll need to use glGetTexImage.
			// Otherwise, you'll need the original CPU data to be stored (not shown here).

			// For now, just create an empty texture copy.
		}

		Texture& operator=(const Texture& other) {
			if (this != &other) {
				// Clean up existing
				if (m_rendererID) {
					glDeleteTextures(1, &m_rendererID);
				}

				m_width = other.m_width;
				m_height = other.m_height;
				m_channels = other.m_channels;
				m_internalFormat = other.m_internalFormat;
				m_dataFormat = other.m_dataFormat;

				glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
				glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

				// Same note as above — you'd need the original data to copy pixels.
			}
			return *this;
		}

		Texture(Texture&& other) noexcept
			: m_rendererID(other.m_rendererID),
			  m_width(other.m_width),
			  m_height(other.m_height),
			  m_channels(other.m_channels),
			  m_internalFormat(other.m_internalFormat),
			  m_dataFormat(other.m_dataFormat)
		{
			other.m_rendererID = 0;
		}

		Texture& operator=(Texture&& other) noexcept
		{
			if (this != &other)
			{
				if (m_rendererID)
				{
					glDeleteTextures(1, &m_rendererID);
				}

				m_rendererID = other.m_rendererID;
				m_width = other.m_width;
				m_height = other.m_height;
				m_channels = other.m_channels;
				m_internalFormat = other.m_internalFormat;
				m_dataFormat = other.m_dataFormat;

				other.m_rendererID = 0;
			}
			return *this;
		}

		void Bind(u32 textureUnit = 0) const;
		void Unbind() const;
		
	private:
		int m_width = 0;
		int m_height = 0;
		int m_channels = 0;
		
		u32 m_internalFormat = 0;
		u32 m_dataFormat = 0;
		u32 m_rendererID = 0;
	};
}
