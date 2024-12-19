#pragma once

#include "Core/Base.h"

namespace zn
{
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();

		void Bind(uint32_t textureUnit = 0) const;
		void Unbind() const;

		bool IsLoaded() const { return m_isLoaded;  }

	private:
		bool m_isLoaded = false;

		uint32_t m_rendererID;
		
		int m_width;
		int m_height;
		int m_channels;
	};
}
