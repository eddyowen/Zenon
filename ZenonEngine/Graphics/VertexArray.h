#pragma once

#include "Core/Base.h"
#include "Core/Assert.h"

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
		uint32_t m_rendererID;
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
		uint32_t m_rendererID;
		uint32_t m_count;
	};

	class VertexBufferLayout
	{
	public:
		struct VertexBufferElement
		{
			unsigned char normalized;
			unsigned int type;
			unsigned int count;

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

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }

		template<typename T>
		void PushElement(unsigned int count)
		{
			ZN_CORE_ASSERT(false);
		}

		template<>
		void PushElement<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FALSE, GL_FLOAT, count });
			m_Stride += count * sizeof(GLfloat);
		}

		template<>
		void PushElement<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_FALSE, GL_FLOAT, count});
			m_Stride += count * sizeof(GLuint);
		}

		template<>
		void PushElement<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_FALSE, GL_FLOAT, count });
			m_Stride += count * sizeof(GLubyte);
		}

	private:
		template<typename T>
		struct InvalidPushType : std::false_type {};

		unsigned int m_Stride;
		std::vector<VertexBufferElement> m_Elements;
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
		uint32_t m_rendererID;
	};
}