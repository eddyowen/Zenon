#pragma once

#include "Core/Base.h"

namespace zn
{
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();

		void Bind() const;
		void Unbind() const;

		bool IsLoaded() const { return m_IsLoaded;  }

	private:
		bool m_IsLoaded = false;

		unsigned int m_RendererID;
		
		int m_Width;
		int m_Height;
		int m_Channels;
	};
}
