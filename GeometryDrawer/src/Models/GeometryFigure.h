#pragma once

#include "OpenGL/IndexBuffer.h"
#include "glm/glm.hpp"

struct GeometryFigure
{
	IndexBuffer Ib;
	unsigned int Type = 0;
	glm::vec4 Color = { 1.f, 1.f, 1.f, 1.f };
};

