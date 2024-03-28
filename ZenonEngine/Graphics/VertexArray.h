#pragma once

#include "Core/Base.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace zn
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		void SetData(const void* data, unsigned int size);

		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_RendererID;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		void SetData(const unsigned int* indices, unsigned int count);

		void Bind() const;
		void Unbind() const;
		unsigned int GetCount() const;

	private:
		GLuint m_RendererID;
		unsigned int m_Count;
	};

	class VertexBufferLayout
	{
	public:
		struct VertexBufferElement
		{
			unsigned int type;
			unsigned int count;
			unsigned char normalized;

			static unsigned int GetSizeOfType(unsigned int type)
			{
				switch (type)
				{
					case GL_FLOAT: return sizeof(GLfloat);
					case GL_UNSIGNED_INT: return sizeof(GLuint);
					case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
				}

				return 0;
			}
		};

		VertexBufferLayout() : m_Stride(0) {}

		template<typename T>
		void Push(unsigned int count) 
		{
			if constexpr (std::is_same_v<T, float>) 
			{
				m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
				m_Stride += count * sizeof(GLfloat);
			}
			else if constexpr (std::is_same_v<T, unsigned int>) 
			{
				m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
				m_Stride += count * sizeof(GLuint);
			}
			else if constexpr (std::is_same_v<T, unsigned char>) 
			{
				m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
				m_Stride += count * sizeof(GLubyte);
			}
			else 
			{
				static_assert(std::is_same_v<T, void>, "No specialization provided for this type.");
			}
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }

	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	private:
		GLuint m_RendererID;
	};
}