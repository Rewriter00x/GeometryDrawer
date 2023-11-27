#pragma once

#include <string>
#include <vector>
#include "CommandType.h"

struct ParsedCommand
{
	static ParsedCommand From(const std::wstring& command);

	CommandType Type = CommandType::None;
	std::wstring Value;
	std::vector<float> OptionalData;

};