#pragma once
#include <EngineContext.h>

namespace REC
{
	class GameObject;
}

namespace Game
{
	class Player;

	class LevelUI final
	{
	public:
		explicit LevelUI(const REC::EngineContext& context, Player* p1, Player* p2);
		~LevelUI() = default;

		LevelUI(const LevelUI&) = delete;
		LevelUI(LevelUI&&) noexcept = delete;
		LevelUI& operator=(const LevelUI&) = delete;
		LevelUI& operator=(LevelUI&&) noexcept = delete;

	private:
		REC::GameObject* m_UI{ nullptr };
	};
}