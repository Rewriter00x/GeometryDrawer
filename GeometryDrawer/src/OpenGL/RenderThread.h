#pragma once

#include <vector>
#include <unordered_map>

#include "GeometryFigure.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Point.h"

class GLFWwindow;

class RenderThread
{
public:
	RenderThread();
	~RenderThread();

	void AddPoint(const std::string& name, const Point& point);
	void AddDebugData();

	void Run() const;


private:
	struct PointInfo
	{
		Point point;
		unsigned int location;
	};

	void DrawPoints() const;

	Renderer renderer;

	GLFWwindow* m_Window = nullptr;

	VertexArray* m_Va = nullptr;
	VertexBuffer* m_Vb = nullptr;
	Shader* m_Shader = nullptr;

	std::vector<GeometryFigure*> m_Figures;
	std::vector<Point> m_Points;
	std::unordered_map<std::string, PointInfo> m_PointInfos;
	
};

