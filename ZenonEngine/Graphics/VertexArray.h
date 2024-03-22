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
		unsigned int m_Size;
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
		void Push(unsigned int count) {
			// Static assert to cause a compile-time error if this template is instantiated with a type
			// for which we do not provide a specialization below.
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += count * sizeof(GLfloat);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * sizeof(GLuint);
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += count * sizeof(GLubyte);
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