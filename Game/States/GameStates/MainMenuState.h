#pragma once
#include <State/GameState.h>

namespace REC
{
	class InputBinding;
}

namespace Game
{
	class MainMenuState : public REC::GameState
	{
	public:
		MainMenuState(const REC::EngineContext& context);
		virtual ~MainMenuState() = default;

		MainMenuState(const MainMenuState&) = delete;
		MainMenuState(MainMenuState&&) noexcept = delete;
		MainMenuState& operator=(const MainMenuState&) = delete;
		MainMenuState& operator=(MainMenuState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
		REC::InputBinding* m_pStartGameInputBinding{ nullptr };
	};
}