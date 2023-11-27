#include "Parser.h"
#include "OpenGL/RenderThread.h" 
#include "glm/glm.hpp"
#include <unordered_set>
#include <iostream>
#include <sstream>

extern RenderThread* rt;

static const std::unordered_set<std::wstring> addCommands = {
	L"додати",
};

static const std::unordered_set<std::wstring> buildCommands = {
	L"побудувати", L"провести",
};

static const std::vector<std::wstring> pointValue = {
	L"точк",
};

static bool HasPart(const std::wstring& command, const std::vector<std::wstring>& tokens)
{
	for (const std::wstring& token : tokens)
	{
		if (command.find(token) != std::wstring::npos)
		{
			return true;
		}
	}
	return false;
}

static CommandType GetType(const std::wstring& token)
{
	if (addCommands.find(token) != addCommands.end())
	{
		return CommandType::AddPoint;
	}
	if (buildCommands.find(token) != buildCommands.end())
	{
		return CommandType::DrawFigure;
	}
	return CommandType::None;
}

static ParsedCommand ParseAddPoint(const std::vector<std::wstring>& tokens)
{
	if (tokens.size() != 5 || !HasPart(tokens[1], pointValue) || tokens[2].size() != 1)
	{
		return ParsedCommand{};
	}

	std::vector<float> pointCoords;
	try {
		pointCoords.push_back(stof(tokens[3]));
		pointCoords.push_back(stof(tokens[4]));
	}
	catch (...) {
		return ParsedCommand{};
	}
	return ParsedCommand{ CommandType::AddPoint, tokens[2], pointCoords };
}

static ParsedCommand ParseDrawFigure(const std::vector<std::wstring>& tokens)
{
	return ParsedCommand{ CommandType::DrawFigure };
}

static ParsedCommand ParseVector(const std::vector<std::wstring>& tokens)
{
	if (tokens.empty())
	{
		return ParsedCommand{};
	}

	CommandType Type = GetType(tokens[0]);
	switch (Type)
	{
	case CommandType::AddPoint:
		return ParseAddPoint(tokens);

	case CommandType::DrawFigure:
		return ParseDrawFigure(tokens);
		
	case CommandType::None:
	default:
		return ParsedCommand{};

	}
}

ParsedCommand ParsedCommand::From(const std::wstring& command)
{
	std::wstringstream ss(command);
	std::wstring token;
	std::vector<std::wstring> tokens;
	while (ss >> token)
	{
		if (!token.empty())
		{
			tokens.push_back(token);
		}
	}
	return ParseVector(tokens);
}
