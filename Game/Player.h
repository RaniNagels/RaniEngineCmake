#pragma once

namespace REC
{
	struct PlayerDescriptor
	{

	};

	class Player final
	{
	public:
		explicit Player() = default;
		~Player() = default;

		Player(const Player& other) = delete;
		Player(Player&& other) = delete;
		Player& operator=(const Player& other) = delete;
		Player& operator=(Player&& other) = delete;
	};
}