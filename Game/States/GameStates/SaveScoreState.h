#pragma once
#include <State/GameState.h>

namespace Game
{
	class SaveScoreState : public REC::GameState
	{
	public:
		SaveScoreState(const REC::EngineContext& context);
		virtual ~SaveScoreState() = default;

		SaveScoreState(const SaveScoreState&) = delete;
		SaveScoreState(SaveScoreState&&) noexcept = delete;
		SaveScoreState& operator=(const SaveScoreState&) = delete;
		SaveScoreState& operator=(SaveScoreState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
	};
}