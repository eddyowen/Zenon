#include "VertexArray.h"

namespace zn
{
	// Explicit template instantiation
	template void VertexBufferLayout::Push<float>(unsigned int count);
	template void VertexBufferLayout::Push<unsigned int>(unsigned int count);
	template void VertexBufferLayout::Push<unsigned char>(unsigned int count);

	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_RendererID);
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::SetData(const void* data, unsigned int size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	IndexBuffer::IndexBuffer()
	{
		glGenBuffers(1, &m_RendererID);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void IndexBuffer::SetData(const unsigned int* indices, unsigned int count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	unsigned int IndexBuffer::GetCount() const
	{
		return m_Count;
	}

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
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
		glBindVertexArray(m_RendererID);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}