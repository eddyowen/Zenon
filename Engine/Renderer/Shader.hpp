#pragma once

#include "Core/Base.hpp"
#include "Math/Math.hpp"

namespace zn
{
	class Shader 
	{
	public:
		Shader(const c8* vertCode, const c8* fragCode);
		~Shader();

		Shader(const Shader& other) = delete;
		Shader& operator=(const Shader& other) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
		
		void Bind() const;
		void Unbind() const;

		void SetInt(const String& name, i32 value) const;
		void SetFloat(const String& name, f32 value) const;
		void SetVec3(const String& name, const math::v3& value) const;
		void SetVec4(const String& name, const math::v4& value) const;
		void SetMat4(const String& name, const math::m4& value) const;

	private:
		static void CheckCompileErrors(u32 rendererId, const String& type);
		
		uint32_t m_rendererID;
	};
}
