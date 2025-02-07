#pragma once

#include "Core/Base.h"

namespace zn
{
	class Texture
	{
	public:
		Texture(
			unsigned char* data,
			int width,
			int height,
			int channels,
			unsigned int internalFormat,
			unsigned int dataFormat);
		
		~Texture();

		void Bind(uint32_t textureUnit = 0) const;
		void Unbind() const;
		
	private:
		unsigned char* m_data;

		uint32_t m_rendererID;
		
		int m_width;
		int m_height;
		int m_channels;
		
		unsigned int m_internalFormat;
		unsigned int m_dataFormat;
	};
}
