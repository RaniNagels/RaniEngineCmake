#pragma once
#include <State/IState.h>
#include <EngineContext.h>
#include <Components/IListener.h>
#include <vector>

namespace REC
{
	class Scene;
	using SceneId = unsigned int;

	class GameState : public IState, public IListener
	{
	public:
		virtual ~GameState() = default;

		GameState(const GameState& other) = delete;
		GameState(GameState&& other) noexcept = delete;
		GameState& operator=(const GameState& other) = delete;
		GameState& operator=(GameState&& other) noexcept = delete;

		virtual void Notify(REC::Event* event) final override { NotifiedEvents.push_back(event); } // do not overwrite
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) = 0;

		std::vector<Event*> GetNotifiedEvents() const { return NotifiedEvents; }
		void ClearNotifiedEvents() { NotifiedEvents.clear(); }

	protected:
		explicit GameState(const EngineContext& context) : IState(), m_Context(context) {}
		Scene* GetScene() const { return m_pScene; }
		EngineContext GetContext() const { return m_Context; }

		Scene* CreateScene(SceneId id);
		//// can only be set once!
		//void SetScene(Scene* scene) { if (!m_IsSceneSet) m_pScene = scene; m_IsSceneSet = true; }

	private:
		virtual std::optional<std::unique_ptr<REC::IState>> Update(float) final override { return {}; };

		Scene* m_pScene{ nullptr };
		EngineContext m_Context{ nullptr };

		std::vector<Event*> NotifiedEvents{};
	};

	class EmptyState : public GameState
	{
	public:
		explicit EmptyState(const EngineContext& context) : GameState(context) {}
		virtual ~EmptyState() = default;

		EmptyState(const EmptyState& other) = delete;
		EmptyState(EmptyState&& other) noexcept = delete;
		EmptyState& operator=(const EmptyState& other) = delete;
		EmptyState& operator=(EmptyState&& other) noexcept = delete;

		virtual void Enter() override {};
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event*) override { return {}; }
		virtual void Exit() override {};
	};
}