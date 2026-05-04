#pragma once
#include <memory>
#include <optional>

namespace REC
{
	class IState
	{
	public:
		virtual ~IState() = default;

		IState(const IState& other) = delete;
		IState(IState&& other) noexcept = delete;
		IState& operator=(const IState& other) = delete;
		IState& operator=(IState&& other) noexcept = delete;

		virtual void Enter() = 0;
		virtual std::optional<std::unique_ptr<IState>> Update(float deltaTime) = 0;
		virtual void Exit() = 0;

	protected:
		explicit IState() = default;
	};

	class GameObject;
	class GameObjectState : public IState
	{
	public :
		explicit GameObjectState(GameObject* owner) : IState(), m_pOwner(owner) {}
		virtual ~GameObjectState() = default;

		GameObjectState(const GameObjectState& other) = delete;
		GameObjectState(GameObjectState&& other) noexcept = delete;
		GameObjectState& operator=(const GameObjectState& other) = delete;
		GameObjectState& operator=(GameObjectState&& other) noexcept = delete;

	protected:
		GameObject* GetGameObject() const { return m_pOwner; }

	private:
		GameObject* m_pOwner{ nullptr };
	};
}