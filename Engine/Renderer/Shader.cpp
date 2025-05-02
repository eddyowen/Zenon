#include "Shader.h"

#include "Core/Log.h"
#include "FileSystem/FileSystem.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

namespace zn
{
	Shader::Shader(const c8* vertCode, const c8* fragCode)
	{
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertCode, nullptr);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragCode, nullptr);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");

		m_rendererID = glCreateProgram();
		glAttachShader(m_rendererID, vertex);
		glAttachShader(m_rendererID, fragment);
		glLinkProgram(m_rendererID);
		CheckCompileErrors(m_rendererID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::CheckCompileErrors(u32 rendererId, const String& type)
	{
		GLint success;
		GLchar infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(rendererId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(rendererId, 1024, nullptr, infoLog);
				ZN_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog)
			}
		}
		else
		{
			glGetProgramiv(rendererId, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(rendererId, 1024, nullptr, infoLog);
				ZN_CORE_ERROR("ERROR::SHADER_LINKING_ERROR of type: " + type + "\n" + infoLog)
			}
		}
	}

	void Shader::Bind() const
	{
		glUseProgram(m_rendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const String& name, i32 value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetFloat(const String& name, f32 value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetVec3(const String& name, const math::v3& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void Shader::SetVec4(const String& name, const math::v4& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void Shader::SetMat4(const String& name, const math::m4& value) const
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}
