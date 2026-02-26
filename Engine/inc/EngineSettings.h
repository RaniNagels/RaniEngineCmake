#pragma once
#include <string>

namespace REC
{
	struct EngineSettings final
	{
		uint8_t frameRate = 60;
		std::string windowTitle = "REC Engine";
		uint16_t windowWidth = 800;
		uint16_t windowHeight = 600;
	};
}