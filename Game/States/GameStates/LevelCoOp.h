#pragma once
#include "LevelState.h"

namespace Game
{
	class LevelCoOp : public LevelState
	{
	public:
		LevelCoOp(const REC::EngineContext& context);
		virtual ~LevelCoOp() = default;

		LevelCoOp(const LevelCoOp&) = delete;
		LevelCoOp(LevelCoOp&&) noexcept = delete;
		LevelCoOp& operator=(const LevelCoOp&) = delete;
		LevelCoOp& operator=(LevelCoOp&&) noexcept = delete;

		virtual void Enter() override;
		virtual void Exit() override;

	private:
	};
}