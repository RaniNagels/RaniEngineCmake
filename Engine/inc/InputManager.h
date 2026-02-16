#pragma once
#include "Singleton.h"

namespace REC
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
	};

}
