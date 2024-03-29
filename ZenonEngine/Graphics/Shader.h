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
		Shader(const std::string& name, const char* vertexPath, const char* fragmentPath);
		~Shader() = default;

		void Load(const char* vertexPath, const char* fragmentPath);

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		
	private:
		void CheckCompileErrors(GLuint shader, const std::string& type);

		std::string m_Name;
		GLuint m_Id;
	};
}
