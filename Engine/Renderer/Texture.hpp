#pragma once

#include "Core/Base.hpp"
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

		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

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
