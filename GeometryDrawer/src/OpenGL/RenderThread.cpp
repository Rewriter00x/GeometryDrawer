#include "RenderThread.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

RenderThread::RenderThread()
{
    /* Initialize the library */
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(1920, 1080, "GeometryDrawer", NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "GLEW_NOT_OK\n";
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10);

    std::cout << glGetString(GL_VERSION) << std::endl;

    m_Va = new VertexArray;
    m_Vb = new VertexBuffer;
    m_Shader = new Shader("res/shaders/Basic.shader");
}

RenderThread::~RenderThread()
{
    delete m_Va;
    delete m_Vb;
    delete m_Shader;
    for (GeometryFigure* figure : m_Figures)
    {
        delete figure;
    }
    glfwTerminate(); // destroys window
}

void RenderThread::AddPoint(char name, const Point& point)
{
    if (m_PointInfos.find(name) != m_PointInfos.end())
    {
        m_Points[m_PointInfos[name].location] = point;
        m_PointInfos[name].point = point;
    }
    else
    {
        m_Points.push_back(point);
        m_PointInfos[name] = PointInfo(point, m_Points.size() - 1);
    }
    m_Vb->Refill(&m_Points.front(), m_Points.size() * 2 * sizeof(float));
}

bool RenderThread::AddFigure(const std::string& points, const glm::vec4& color)
{
    if (points.empty())
    {
        return false;
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < points.size(); ++i)
    {
        char name = points[i];
        if (m_PointInfos.find(name) == m_PointInfos.end())
        {
            return false;
        }

        if (indices.size() >= 3)
        {
            indices.push_back(m_PointInfos[points[i - 1]].location);
            indices.push_back(m_PointInfos[name].location);
            indices.push_back(m_PointInfos[points[0]].location);
        }
        else
        {
            indices.push_back(m_PointInfos[name].location);
        }
    }
    m_Figures.emplace_back(new GeometryFigure{ IndexBuffer(&indices.front(), indices.size()), GetStringFigureType(points), color });
    return true;
}

void RenderThread::AddDebugData()
{
#if _DEBUG
    AddPoint('A', { 400.f, 400.f });
    AddPoint('B', { 300.f, 300.f });
    AddPoint('B', { 700.f, 700.f });
    AddPoint('C', { 1000.f, 400.f });
    AddPoint('D', { 700.f, 100.f });
    AddPoint('E', { 400.f, 100.f });
    AddPoint('F', { 300.f, 250.f });
    /*AddFigure("ABC");
    AddFigure("AD");
    AddFigure("CD");
    AddFigure("ABCDEF");*/
#endif
}

void RenderThread::AddPointQueue(char name, const Point& point)
{
    m_Events.push_back(new AddPointEvent(name, point));
}

void RenderThread::AddFigureQueue(const std::string& points, const glm::vec4& color)
{
    m_Events.push_back(new AddFigureEvent(points, color));
}

void RenderThread::Run()
{
    VertexBufferLayout layout;
    layout.Push<GL_FLOAT>(2);
    m_Va->AddBuffer(*m_Vb, layout);

    glm::vec4 color{ .2f, .3f, .8f, 1.f };

    m_Va->Unbind();
    m_Shader->Unbind();
    m_Vb->Unbind();

    glm::mat4 proj;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_Window))
    {
        pollEvents();
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        proj = glm::ortho(0.f, (float)width, 0.f, (float)height);

        /* Render here */
        renderer.Clear();
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_PM", proj);
        m_Shader->SetUniform4fv("u_Color", color);

        for (GeometryFigure* figure : m_Figures)
        {
            m_Shader->Bind();
            m_Shader->SetUniform4fv("u_Color", figure->Color);

            renderer.Draw(*m_Va, figure->Ib, *m_Shader, figure->Type);
        }

        DrawPoints();

        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

unsigned int RenderThread::GetStringFigureType(const std::string& type)
{
    size_t size = type.size();
    if (size >= 3)
    {
        return GL_TRIANGLES;
    }
    switch (size)
    {
    case 1:
        return GL_POINTS;
    case 2:
        return GL_LINES;
    default:
        throw 0;
    }
}

void RenderThread::DrawPoints() const
{
    for (unsigned int i = 0; i < m_Points.size(); ++i)
    {
        const Point& point = m_Points[i];
        m_Shader->Bind();
        m_Shader->SetUniform4fv("u_Color", { 1.f, 0.f, 0.f, 1.f });
        renderer.Draw(*m_Va, IndexBuffer(&i, 1), *m_Shader, GL_POINTS);
    }
}

void RenderThread::pollEvents()
{
    while (!m_Events.empty())
    {
        UpdateEvent* evt = m_Events.back();
        evt->Exec(this);
        delete evt;
        m_Events.pop_back();
    }
}

RenderThread::AddPointEvent::AddPointEvent(char name, const Point& point)
    : m_Name(name), m_Point(point)
{
}

void RenderThread::AddPointEvent::Exec(RenderThread* thread)
{
    thread->AddPoint(m_Name, m_Point);
}

RenderThread::AddFigureEvent::AddFigureEvent(const std::string& value, const glm::vec4& color)
    : m_Value(value), m_Color(color)
{
}

void RenderThread::AddFigureEvent::Exec(RenderThread* thread)
{
    thread->AddFigure(m_Value, m_Color);
}
