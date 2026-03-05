#pragma once
#include <vector>
#include <string>

namespace REC
{
	struct AnimationInfo final
	{
		std::string animationKey{};
		std::vector<std::string> frameKeys{}; // must be in the correct order
		float duration{}; // how long does the complete animation take in seconds
		bool loop{};
	};
}