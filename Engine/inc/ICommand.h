#pragma once

namespace REC
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		virtual void Execute() = 0;
	};
}