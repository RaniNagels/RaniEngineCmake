#pragma once
#include <State/GameState.h>
#include <vector>
#include <memory>

namespace REC
{
	class InputBinding;
}

namespace Game
{
	class UIMarker;
	class PlayerMenuState : public REC::GameState
	{
	public:
		PlayerMenuState(const REC::EngineContext& context);
		virtual ~PlayerMenuState() = default;

		PlayerMenuState(const PlayerMenuState&) = delete;
		PlayerMenuState(PlayerMenuState&&) noexcept = delete;
		PlayerMenuState& operator=(const PlayerMenuState&) = delete;
		PlayerMenuState& operator=(PlayerMenuState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
		std::unique_ptr<UIMarker> m_pUIMarker{ nullptr };
		std::vector<REC::InputBinding*> m_InputBindings{};
	};
}