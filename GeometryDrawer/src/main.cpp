#include <iostream>
#include <thread>
#include <locale>
#include "windows.h"

#include "OpenGL/RenderThread.h"
#include "glm/glm.hpp"
#include "Parser.h"

RenderThread* rt = nullptr;

static void AddPoint(const ParsedCommand& command)
{
    const std::vector<float>& data = command.OptionalData;
    Point point{ data[0], data[1] };
    rt->AddPointQueue(command.Value[0], point);
}

static void DrawFigure(const ParsedCommand& command)
{
    std::string value;
    size_t size = command.Value.size();
    value.resize(size);
    wcstombs_s(&size, &value[0], value.size() + 1, command.Value.c_str(), command.Value.size());
    const std::vector<float>& data = command.OptionalData;
    rt->AddFigureQueue(value, glm::vec4(data[0], data[1], data[2], data[3]));
}

static void ExecuteCommand(const ParsedCommand& command)
{
    switch (command.Type)
    {
    case CommandType::AddPoint:
        AddPoint(command);
        break;
        
    case CommandType::DrawFigure:
        DrawFigure(command);
        break;

    case CommandType::None:
    default:
        std::wcout << L"Погана команда\n";
        break;

    }
}

int main()
{
    setlocale(LC_CTYPE, "ukr");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::jthread renderThread(
        [] {
            rt = new RenderThread;
            //rt->AddDebugData();
            rt->Run();
            delete rt;
        }
    );

    std::wstring input;
    while (true)
    {
        getline(std::wcin, input);
        if (input == L"вихід")
        {
            break;
        }
        ExecuteCommand(ParsedCommand::From(input));
    }

    return 0;
}