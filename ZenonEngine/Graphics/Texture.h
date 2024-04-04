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

		bool IsLoaded() const { return m_IsLoaded;  }

	private:
		bool m_IsLoaded = false;

		uint32_t m_RendererID;
		
		int m_Width;
		int m_Height;
		int m_Channels;
	};
}
