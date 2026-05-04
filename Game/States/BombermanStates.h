#pragma once
#include <State/IState.h>
#include <Components/IListener.h>

#include <glm/glm.hpp>
#include <string>

namespace REC
{
	class AnimatedSpriteComponent;
	class LivesComponent;
}

namespace Game
{
	class BombermanDeadState : public REC::GameObjectState
	{
	public:
		explicit BombermanDeadState(REC::GameObject* owner);
		virtual ~BombermanDeadState() = default;

		BombermanDeadState(const BombermanDeadState&) = delete;
		BombermanDeadState(BombermanDeadState&&) noexcept = delete;
		BombermanDeadState& operator=(const BombermanDeadState&) = delete;
		BombermanDeadState& operator=(BombermanDeadState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float deltaTime) override;
		virtual void Exit() override {};

	private:
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
	};

	class BombermanIdleState : public REC::GameObjectState, public REC::IListener
	{
	public:
		explicit BombermanIdleState(REC::GameObject* owner);
		virtual ~BombermanIdleState() = default;

		BombermanIdleState(const BombermanIdleState&) = delete;
		BombermanIdleState(BombermanIdleState&) noexcept = delete;
		BombermanIdleState& operator=(const BombermanIdleState&) = delete;
		BombermanIdleState& operator=(BombermanIdleState&&) noexcept = delete;

		virtual void Notify(REC::Event* event) override;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float deltaTime) override;
		virtual void Exit() override;

	private:
		bool m_HasBeenNotified{ false };
		glm::vec2 m_Direction{};
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
		REC::LivesComponent* m_pLivesComponent{ nullptr };
	};

	class BombermanWalkingState : public REC::GameObjectState, public REC::IListener
	{
	public:
		explicit BombermanWalkingState(REC::GameObject* owner, glm::vec2 dir);
		virtual ~BombermanWalkingState() = default;

		BombermanWalkingState(const BombermanWalkingState&) = delete;
		BombermanWalkingState(BombermanWalkingState&&) noexcept = delete;
		BombermanWalkingState& operator=(const BombermanWalkingState&) = delete;
		BombermanWalkingState& operator=(BombermanWalkingState&&) noexcept = delete;

		virtual void Notify(REC::Event* event) override;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float deltaTime) override;
		virtual void Exit() override;

	private:
		std::string GetAnimationKey(glm::vec2 dir) const;
		void ChangeAnimation();

		bool m_HasBeenNotified{ false };
		bool m_ChangedDirection{ false };
		std::string m_AnimationKey{};
		glm::vec2 m_Direction{};
		REC::AnimatedSpriteComponent* m_pAnimatedSpriteComponent{ nullptr };
		REC::LivesComponent* m_pLivesComponent{ nullptr };
	};
}