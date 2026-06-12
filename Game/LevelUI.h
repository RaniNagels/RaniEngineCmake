#pragma once
#include <EngineContext.h>
#include <string>

namespace REC
{
	class GameObject;
	class Scene;
	using ObjectId = unsigned int;
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

		uint32_t GetPlayer1Score() const;
		uint32_t GetPlayer2Score() const;

	private:
		void CreatePlayerUI(REC::Scene* scene, REC::GameObject* parent, Player* player, REC::ObjectId id, const std::string& prefix, float x, float y);
		REC::GameObject* m_UI{ nullptr };
	};
}