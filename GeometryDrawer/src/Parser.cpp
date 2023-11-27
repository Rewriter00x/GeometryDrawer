#include "Parser.h"
#include "OpenGL/RenderThread.h"
#include <unordered_set>
#include <unordered_map>
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

static const std::vector<std::wstring> figureValue = {
	L"фігур", L"прям", L"трикутник", L"чотирикутник",
};

static std::unordered_map<std::wstring, std::vector<float>> colorValues = {
	{ L"чорн", { 0.f, 0.f, 0.f, 1.f } },
	{ L"біл", { 1.f, 1.f, 1.f, 1.f } },
	{ L"червон", { 1.f, 0.f, 0.f, 1.f } },
	{ L"син", { 0.f, 0.f, 1.f, 1.f } },
	{ L"зелен", { 0.f, 1.f, 0.f, 1.f } },
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

static const std::vector<float>* HasPart(const std::wstring& command, std::unordered_map<std::wstring, std::vector<float>>& tokens)
{
	for (const auto& token : tokens)
	{
		if (command.find(token.first) != std::wstring::npos)
		{
			std::wstring name = token.first;
			return &tokens[name];
		}
	}
	return nullptr;
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
	const std::vector<float>* color;
	if (tokens.size() != 4)
	{
		return ParsedCommand{};
	}
	if (color = HasPart(tokens[1], colorValues))
	{
		if (!HasPart(tokens[2], figureValue))
		{
			return ParsedCommand{};
		}
		return ParsedCommand{ CommandType::DrawFigure, tokens[3], *color};
	}
	return ParsedCommand{};
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
