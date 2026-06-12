#pragma once
#include <State/IState.h>
#include <Components/IListener.h>
#include <Events/Event.h>
#include <memory>

namespace REC
{
	class AnimatedSpriteComponent;
	class LivesComponent;
}

namespace Game
{
	class BalloomFloatingState : public REC::GameObjectState, public REC::IListener
	{
	public:
		explicit BalloomFloatingState(REC::GameObject* owner);
		virtual ~BalloomFloatingState();

		BalloomFloatingState(const BalloomFloatingState&) = delete;
		BalloomFloatingState(BalloomFloatingState&&) noexcept = delete;
		BalloomFloatingState& operator=(const BalloomFloatingState&) = delete;
		BalloomFloatingState& operator=(BalloomFloatingState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float deltaTime) override;
		virtual void Exit() override;

		virtual void Notify(REC::Event* event) override;

	private:
		bool m_DeathNotified{ false };
		bool m_HasBeenNotified{ false };
		bool m_ChangedDirection{ false };
		float m_XDirection{};
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
		REC::LivesComponent* m_pLivesComponent{ nullptr };
	};

	class BalloomDeadState : public REC::GameObjectState
	{
	public:
		explicit BalloomDeadState(REC::GameObject* owner);
		virtual ~BalloomDeadState() = default;

		BalloomDeadState(const BalloomDeadState&) = delete;
		BalloomDeadState(BalloomDeadState&&) noexcept = delete;
		BalloomDeadState& operator=(const BalloomDeadState&) = delete;
		BalloomDeadState& operator=(BalloomDeadState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float deltaTime) override;
		virtual void Exit() override;

	private:
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
		std::unique_ptr<REC::Event> m_pVeryDeathEvent;
	};
}