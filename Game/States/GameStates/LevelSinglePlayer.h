#pragma once
#include "LevelState.h"

namespace Game
{
	class LevelSinglePlayer : public LevelState
	{
	public:
		LevelSinglePlayer(const REC::EngineContext& context);
		virtual ~LevelSinglePlayer() = default;

		LevelSinglePlayer(const LevelSinglePlayer&) = delete;
		LevelSinglePlayer(LevelSinglePlayer&&) noexcept = delete;
		LevelSinglePlayer& operator=(const LevelSinglePlayer&) = delete;
		LevelSinglePlayer& operator=(LevelSinglePlayer&&) noexcept = delete;

		virtual void Enter() override;
		virtual void Exit() override;

	private:
	};
}