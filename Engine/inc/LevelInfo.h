#pragma once
#include <cstdint>
#include <string>
#include <vector>

// TODO: should not be in engine, should be in game

namespace REC
{
	using CellCo = std::pair<uint8_t, uint8_t>;
	using EnemyInfo = std::pair<CellCo, std::string>; // position and type
	using PowerUpInfo = std::pair<uint8_t, std::string>; // index and type

	struct LevelInfo final
	{
		uint8_t id;
		CellCo player1StartCell;
		CellCo player2StartCell;
		std::vector<CellCo> bricks;
		std::vector<EnemyInfo> enemies;
		uint16_t exitBrickIndex;
		PowerUpInfo powerUps;
	};
}