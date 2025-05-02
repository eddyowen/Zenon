#pragma once

#include "Core/Base.h"

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

		void Bind(u32 textureUnit = 0) const;
		void Unbind() const;
		
	private:
		u8* m_data;
		
		int m_width;
		int m_height;
		int m_channels;
		
		u32 m_internalFormat;
		u32 m_dataFormat;
		u32 m_rendererID;
	};
}
