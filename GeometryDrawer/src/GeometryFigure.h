#pragma once

#include "OpenGL/IndexBuffer.h"
#include "glm/glm.hpp"

class GeometryFigure
{
public:
	GeometryFigure();

	inline const IndexBuffer& GetIndexBuffer() const { return m_Ib; }
	inline const glm::vec4& GetColor() const { return m_Color; }

private:
	IndexBuffer m_Ib;
	glm::vec4 m_Color;

};

