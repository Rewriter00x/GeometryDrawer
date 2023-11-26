#pragma once

#include <vector>
#include "GeometryFigure.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

class GLFWwindow;

class RenderThread
{
public:
	RenderThread();
	~RenderThread();

	void Run();

private:
	GLFWwindow* m_Window = nullptr;

	VertexArray* m_Va = nullptr;
	VertexBuffer* m_Vb = nullptr;
	Shader* m_Shader = nullptr;
	std::vector<GeometryFigure*> m_Figures;

	
};

