#pragma once
#include "LevelState.h"

namespace Game
{
	class LevelVersus : public LevelState
	{
	public:
		LevelVersus(const REC::EngineContext& context);
		virtual ~LevelVersus() = default;

		LevelVersus(const LevelVersus&) = delete;
		LevelVersus(LevelVersus&&) noexcept = delete;
		LevelVersus& operator=(const LevelVersus&) = delete;
		LevelVersus& operator=(LevelVersus&&) noexcept = delete;

		virtual void Enter() override;
		virtual void Exit() override;
	private:
	};
}