#pragma once
#include <State/GameState.h>
#include "../Player.h"
#include "../LevelUI.h"
#include <vector>

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

	class LevelState : public REC::GameState
	{
	public:
		LevelState(const REC::EngineContext& context);
		virtual ~LevelState() = default;

		LevelState(const LevelState&) = delete;
		LevelState(LevelState&&) noexcept = delete;
		LevelState& operator=(const LevelState&) = delete;
		LevelState& operator=(LevelState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
		std::vector<std::unique_ptr<Player>> m_Players{};
		std::unique_ptr<LevelUI> m_pLevelUI{};
	};

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