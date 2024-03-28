#include "Shader.h"

#include "Core/Log.h"

namespace zn
{
	Shader::Shader(const std::string& name, const char* vertexPath, const char* fragmentPath)
	{
		m_Name = name;

		Load(vertexPath, fragmentPath);
	}

	void Shader::Load(const char* vertexPath, const char* fragmentPath) 
	{
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			ZN_CORE_ERROR("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ - Shader Namme: " + m_Name);
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		GLuint vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, nullptr);
		glCompileShader(vertex);
		CheckCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, nullptr);
		glCompileShader(fragment);
		CheckCompileErrors(fragment, "FRAGMENT");

		m_Id = glCreateProgram();
		glAttachShader(m_Id, vertex);
		glAttachShader(m_Id, fragment);
		glLinkProgram(m_Id);
		CheckCompileErrors(m_Id, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::CheckCompileErrors(GLuint shader, const std::string& type)
	{
		GLint success;
		GLchar infoLog[1024];

		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				ZN_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				ZN_CORE_ERROR("ERROR::SHADER_LINKING_ERROR of type: " + type + "\n" + infoLog);
			}
		}
	}

	void Shader::Bind() const
	{
		glUseProgram(m_Id);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(m_Id, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}
