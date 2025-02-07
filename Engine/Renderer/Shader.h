#pragma once

#include "Core/Base.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace zn
{
	class Shader 
	{
	public:
		Shader(const char* vertCode, const char* fragCode);
		~Shader() = default;

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetVec4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

	private:
		void CheckCompileErrors(unsigned int rendererId, const std::string& type) const;
		
		uint32_t m_rendererID;
	};
}
