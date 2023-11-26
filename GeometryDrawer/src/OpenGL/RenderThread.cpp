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

#include "GeometryFigure.h"

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
    glPointSize(5);

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
    glfwTerminate();
}

void RenderThread::Run()
{
    float positions[] = {
         480.f,  270.f,
         1440.f, 270.f,
         1440.f, 810.f,
         480.f, 810.f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    float pointPos[] = {
        300.f, 300.f,
    };

    unsigned int pointIds[] = {
        0,
    };

    VertexArray va;
    VertexBuffer vb(pointPos, sizeof(float) * 2);

    m_Vb->Refill(positions, sizeof(float) * 8);

    VertexBufferLayout layout;
    layout.Push<GL_FLOAT>(2);
    m_Va->AddBuffer(*m_Vb, layout);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);
    IndexBuffer pointIb(pointIds, 1);

    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", .2f, .3f, .8f, 1.f);

    m_Va->Unbind();
    m_Shader->Unbind();
    m_Vb->Unbind();
    ib.Unbind();

    Renderer renderer;

    glm::mat4 proj;
    glm::vec4 color{ 0.f, 1.f, 0.f, 1.f };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(m_Window))
    {
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        proj = glm::ortho(0.f, (float)width, 0.f, (float)height);

        /* Render here */
        renderer.Clear();
        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_PM", proj);

        renderer.Draw(*m_Va, ib, *m_Shader, GL_TRIANGLES);
        renderer.Draw(va, pointIb, *m_Shader, GL_POINTS);

        for (GeometryFigure* figure : m_Figures)
        {
            m_Shader->Bind();
            m_Shader->SetUniform4fv("u_Color", figure->GetColor());

            renderer.Draw(*m_Va, figure->GetIndexBuffer(), *m_Shader, GL_TRIANGLES);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}
