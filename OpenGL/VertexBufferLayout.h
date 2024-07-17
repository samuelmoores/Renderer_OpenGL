#pragma once
#include <vector>
#include "GL/glew.h"
#include <iostream>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:   return 4;
			case GL_UNSIGNED_BYTE:  return 1;
		}
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() { m_Stride = 0; }

	template<typename T>
	void Push(unsigned int count)
	{
		std::cout << "no type\n";
	}

	template<>
	void Push<float>(unsigned int count)
	{
		VertexBufferElement vbe;
		vbe.type = GL_FLOAT;
		vbe.count = count;
		vbe.normalized = false;
		m_Elements.push_back(vbe);

		m_Stride += VertexBufferElement::GetSizeOfType(vbe.type) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		VertexBufferElement vbe;
		vbe.type = GL_UNSIGNED_INT;
		vbe.count = count;
		vbe.normalized = false;
		m_Elements.push_back(vbe);

		m_Stride += VertexBufferElement::GetSizeOfType(vbe.type) * count;
	}

	template<>
	void Push<char>(unsigned int count)
	{
		VertexBufferElement vbe;
		vbe.type = GL_UNSIGNED_BYTE;
		vbe.count = count;
		vbe.normalized = true;
		m_Elements.push_back(vbe);

		m_Stride += VertexBufferElement::GetSizeOfType(vbe.type) * count;
	}


	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

	inline const unsigned int GetStride() const { return m_Stride; }

};
