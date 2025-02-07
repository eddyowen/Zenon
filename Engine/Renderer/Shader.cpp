#include "Shader.h"

#include "FileSystem/FileSystem.h"
#include "Core/Log.h"

namespace zn
{
	Shader::Shader(const char* vertCode, const char* fragCode)
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

	void Shader::CheckCompileErrors(unsigned int rendererId, const std::string& type) const
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

	void Shader::SetInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void Shader::SetVec4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(value));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}
