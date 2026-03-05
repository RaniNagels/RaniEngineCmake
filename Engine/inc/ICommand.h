#pragma once
#include <cstdint>

namespace REC
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute(uint8_t playerIndex = -1) = 0;
	};
}