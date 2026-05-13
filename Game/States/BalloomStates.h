#pragma once
#include <State/IState.h>

namespace Game
{

	// TODO: not yet used
	class BalloomFloatingState : public REC::GameObjectState
	{
	public:
		explicit BalloomFloatingState(REC::GameObject* owner);
		virtual ~BalloomFloatingState() = default;

		BalloomFloatingState(const BalloomFloatingState&) = delete;
		BalloomFloatingState(BalloomFloatingState&&) noexcept = delete;
		BalloomFloatingState& operator=(const BalloomFloatingState&) = delete;
		BalloomFloatingState& operator=(BalloomFloatingState&&) noexcept = delete;

		virtual void Enter() override {};
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float deltaTime) override { return {}; }
		virtual void Exit() override {};
	};
}