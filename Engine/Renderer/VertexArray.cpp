#include "VertexArray.hpp"

namespace zn
{
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_rendererID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void VertexBuffer::SetData(const void* data, uSize size)
	{
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer()
	{
		glGenBuffers(1, &m_rendererID);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_rendererID);
	}

	void IndexBuffer::SetData(const unsigned int* indices, uSize count)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_rendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
	{
		Bind();
		vertexBuffer.Bind();

		const Vector<VertexBufferLayout::VertexBufferElement>& elements = layout.GetElements();
		uSize offset = 0;

		for (uSize i = 0; i < elements.size(); i++)
		{
			const VertexBufferLayout::VertexBufferElement& element = elements[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, layout.GetStride(), (const void*)offset);

			offset += element.Count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.Type);
		}
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_rendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}