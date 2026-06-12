#pragma once
#include <State/GameState.h>
#include <memory>

namespace REC
{
	class InputBinding;
}

namespace Game
{
	class UIMarker;
	class GameOverState : public REC::GameState
	{
	public:
		GameOverState(const REC::EngineContext& context, uint32_t scoreP1, uint32_t scoreP2);
		virtual ~GameOverState() = default;

		GameOverState(const GameOverState&) = delete;
		GameOverState(GameOverState&&) noexcept = delete;
		GameOverState& operator=(const GameOverState&) = delete;
		GameOverState& operator=(GameOverState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
		std::unique_ptr<UIMarker> m_pUIMarker{ nullptr };
		std::vector<REC::InputBinding*> m_InputBindings{};

		uint32_t m_ScoreP1{ 0 };
		uint32_t m_ScoreP2{ 0 };
	};
}