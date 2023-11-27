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
	void AddDebugData();

	void AddPointQueue(char name, const Point& point);
	void AddFigureQueue(const std::string& points, const glm::vec4& color);

	void Run();


private:
	class UpdateEvent
	{
	public:
		virtual ~UpdateEvent() {}
		virtual void Exec(RenderThread* thread) = 0;;
	};

	class AddPointEvent : public UpdateEvent
	{
	public:
		AddPointEvent(char name, const Point& point);
		virtual void Exec(RenderThread* thread) override;
		
	private:
		char m_Name;
		Point m_Point;
	};

	class AddFigureEvent : public UpdateEvent
	{
	public:
		AddFigureEvent(const std::string& value, const glm::vec4& color);
		virtual void Exec(RenderThread* thread) override;

	private:
		std::string m_Value;
		glm::vec4 m_Color;
	};

	struct PointInfo
	{
		Point point;
		unsigned int location;
	};

	static unsigned int GetStringFigureType(const std::string& type);

	void AddPoint(char name, const Point& point);
	bool AddFigure(const std::string& points, const glm::vec4& color);

	void DrawPoints() const;

	void pollEvents();

	Renderer renderer;

	GLFWwindow* m_Window = nullptr;

	VertexArray* m_Va = nullptr;
	VertexBuffer* m_Vb = nullptr;
	Shader* m_Shader = nullptr;

	std::vector<GeometryFigure*> m_Figures;
	std::vector<Point> m_Points;
	std::unordered_map<char, PointInfo> m_PointInfos;

	std::vector<UpdateEvent*> m_Events;
	
};

