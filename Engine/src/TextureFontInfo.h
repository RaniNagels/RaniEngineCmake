#pragma once
#include <unordered_map>

namespace REC
{
	struct TextureFontInfo
	{
		std::unordered_map<char, std::string> glyphs;
	};
}