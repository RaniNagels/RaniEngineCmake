#pragma once
#include <State/GameState.h>

namespace REC
{
	class InputBinding;
}

namespace Game
{
	class MarkerComponent;
	class GameOverState : public REC::GameState
	{
	public:
		GameOverState(const REC::EngineContext& context);
		virtual ~GameOverState() = default;

		GameOverState(const GameOverState&) = delete;
		GameOverState(GameOverState&&) noexcept = delete;
		GameOverState& operator=(const GameOverState&) = delete;
		GameOverState& operator=(GameOverState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
		MarkerComponent* m_pMarkerComponent{ nullptr };
		std::vector<REC::InputBinding*> m_InputBindings{};
	};
}