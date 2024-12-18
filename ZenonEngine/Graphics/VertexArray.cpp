#include "VertexArray.h"

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

	void VertexBuffer::SetData(const void* data, unsigned int size)
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

	void IndexBuffer::SetData(const unsigned int* indices, unsigned int count)
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

	unsigned int IndexBuffer::GetCount() const
	{
		return m_count;
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

		const std::vector<VertexBufferLayout::VertexBufferElement>& elements = layout.GetElements();
		unsigned int offset = 0;

		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const VertexBufferLayout::VertexBufferElement& element = elements[i];

			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);

			offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
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