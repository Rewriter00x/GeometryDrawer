#pragma once

#include <vector>
#include <unordered_map>

#include "Models/GeometryFigure.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Models/Point.h"

class GLFWwindow;

class RenderThread
{
public:
	RenderThread();
	~RenderThread();

	void AddPoint(char name, const Point& point);
	bool AddFigure(const std::string& points);
	void AddDebugData();

	void Run() const;


private:
	struct PointInfo
	{
		Point point;
		unsigned int location;
	};

	static unsigned int GetStringFigureType(const std::string& type);

	void DrawPoints() const;


	Renderer renderer;

	GLFWwindow* m_Window = nullptr;

	VertexArray* m_Va = nullptr;
	VertexBuffer* m_Vb = nullptr;
	Shader* m_Shader = nullptr;

	std::vector<GeometryFigure*> m_Figures;
	std::vector<Point> m_Points;
	std::unordered_map<char, PointInfo> m_PointInfos;
	
};

