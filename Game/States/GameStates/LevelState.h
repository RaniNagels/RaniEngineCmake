#pragma once
#include <State/GameState.h>
#include "../../Player.h"

namespace Game
{
	enum class PlayerMode
	{
		SinglePlayer,
		CoOp,
		Versus
	};

	class LevelUI;
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

	protected:
		void CreateLevelUI(Player* player1, Player* player2);
		Player* AddPlayer(const PlayerDescriptor& descriptor);
		REC::GameObject* GetPlayfield() const { return m_pGridObject; }
		void SetPlayerMode(PlayerMode mode) { m_PlayerMode = mode; }

	private:
		void ResetLevel();
		void LoadLevel();
		void DisablePlayers();
		void EnablePlayers();

		void CreateSoftBlock(uint8_t row, uint8_t col);

		REC::GameObject* m_pGridObject{ nullptr };
		std::vector<std::unique_ptr<Player>> m_Players{};
		std::unique_ptr<LevelUI> m_pLevelUI{ nullptr };
		PlayerMode m_PlayerMode{ PlayerMode::SinglePlayer };
	};
}