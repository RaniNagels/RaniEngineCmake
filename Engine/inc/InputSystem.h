#pragma once
#include "Singleton.h"

namespace REC
{
	class InputSystem final : public Singleton<InputSystem>
	{
	public:
		bool ProcessInput();
	};

}
