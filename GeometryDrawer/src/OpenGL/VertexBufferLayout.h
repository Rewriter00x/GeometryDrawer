#pragma once

#include <vector>

#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type = 0;
	unsigned int count = 0;
	unsigned char normalized = 0;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: 
			return sizeof(float);
		case GL_UNSIGNED_INT:
			return sizeof(unsigned int);
		default:
			ASSERT(false);
			return 0;
		}
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() {}

	template<unsigned int T>
	void Push(unsigned int count)
	{
		m_Elements.push_back({ T, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(T) * count;
	}

	const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	unsigned int GetStride() const { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride = 0;

};

