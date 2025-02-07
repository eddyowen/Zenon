#include "Texture.h"

#include "Core/Log.h"
#include "FileSystem/FileSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zn
{
	Texture::Texture(const std::string& path)
	{
		if (!FileSystem::Exists(path))
		{
			ZN_CORE_ERROR("[ERROR][Texture::Texture] File {} does not exist", path)
			return;
		}
		
		FileSystem::Path fullPath = FileSystem::GetFullPath(path).value();
		
		stbi_set_flip_vertically_on_load(1);

		int width, height, channels;

		stbi_uc* data = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 0);
		if (data)
		{
			m_isLoaded = true;

			m_width = width;
			m_height = height;
			m_channels = channels;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
			glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			ZN_CORE_ERROR("Failed to load texture: {0}", path)
		}

		stbi_image_free(data);
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


