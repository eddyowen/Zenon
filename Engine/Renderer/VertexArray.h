#pragma once

#include "Core/Base.h"
#include "Core/Assert.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zn
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		VertexBuffer(const VertexBuffer& other) = delete;
		VertexBuffer(VertexBuffer&& other) noexcept = delete;
		
		VertexBuffer& operator=(const VertexBuffer& other) = delete;
		VertexBuffer& operator=(VertexBuffer&& other) noexcept = delete;

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uSize size);

	private:
		u32 m_rendererID;
	};

	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer& other) = delete;
		IndexBuffer(IndexBuffer&& other) noexcept = delete;
		
		IndexBuffer& operator=(const IndexBuffer& other) = delete;
		IndexBuffer& operator=(IndexBuffer&& other) noexcept = delete;

		void Bind() const;
		void Unbind() const;

		void SetData(const unsigned int* indices, uSize count);
		uSize GetCount() const { return m_count; }

	private:
		u32 m_rendererID;
		uSize m_count;
	};

	class VertexBufferLayout
	{
	public:
		struct VertexBufferElement
		{
			unsigned char Normalized;
			unsigned int Type;
			uSize Count;

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

		VertexBufferLayout() : m_stride(0) {}

		[[nodiscard]]
		const Vector<VertexBufferElement>& GetElements() const { return m_elements; }
		
		[[nodiscard]]
		uSize GetStride() const { return m_stride; }

		template<typename T>
		void PushElement(uSize count)
		{
			ZN_ASSERT(false);
		}

		template<>
		void PushElement<f32>(uSize count)
		{
			m_elements.push_back({ GL_FALSE, GL_FLOAT, count });
			m_stride += count * sizeof(GLfloat);
		}

		template<>
		void PushElement<unsigned int>(uSize count)
		{
			m_elements.push_back({ GL_FALSE, GL_UNSIGNED_INT, count});
			m_stride += count * sizeof(GLuint);
		}

		template<>
		void PushElement<unsigned char>(uSize count)
		{
			m_elements.push_back({ GL_FALSE, GL_UNSIGNED_BYTE, count });
			m_stride += count * sizeof(GLubyte);
		}

	private:
		Vector<VertexBufferElement> m_elements;
		uSize m_stride;
	};

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray& other) = delete;
		VertexArray(VertexArray&& other) noexcept = delete;
		
		VertexArray& operator=(const VertexArray& other) = delete;
		VertexArray& operator=(VertexArray&& other) noexcept = delete;

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

	private:
		u32 m_rendererID;
	};
}