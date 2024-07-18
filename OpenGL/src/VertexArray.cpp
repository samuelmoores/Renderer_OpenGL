#include <iostream>
#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_Renderer_ID);
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_Renderer_ID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_Renderer_ID);

}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));

}
