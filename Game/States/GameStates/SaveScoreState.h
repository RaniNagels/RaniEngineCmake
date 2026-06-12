#pragma once
#include <State/GameState.h>
#include <memory>
#include <vector>
#include <ComponentDescriptors.h>
#include <GameObject.h>

namespace REC
{
	class InputBinding;
	class TextRenderComponent;
}

namespace Game
{
	class UIMarker;
	class SaveScoreState : public REC::GameState
	{
	public:
		SaveScoreState(const REC::EngineContext& context, uint32_t scoreP1, uint32_t scoreP2);
		virtual ~SaveScoreState() = default;

		SaveScoreState(const SaveScoreState&) = delete;
		SaveScoreState(SaveScoreState&&) noexcept = delete;
		SaveScoreState& operator=(const SaveScoreState&) = delete;
		SaveScoreState& operator=(SaveScoreState&&) noexcept = delete;

		virtual void Enter() override;
		virtual std::optional<std::unique_ptr<REC::GameState>> OnEvent(REC::Event* event) override;
		virtual void Exit() override;

	private:
		void PrintLetter(REC::TextDescriptor& desc, REC::GameObjectDescriptor& goDesc, float x, float y, char letter);
		void OpenSaveFile();
		void SaveFile();

		std::unique_ptr<UIMarker> m_pUIMarker{ nullptr };
		std::vector<REC::InputBinding*> m_InputBindings{};

		uint32_t m_ScoreP1{ 0 };
		uint32_t m_ScoreP2{ 0 };

		std::vector<char> m_Letters{ 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '<' };

		bool m_FirstPlayer{ true };
		REC::TextRenderComponent* m_pP1Name{ nullptr };
		REC::TextRenderComponent* m_pP2Name{ nullptr };

		// selection
		REC::TextRenderComponent* m_pP1Letter{ nullptr };
		REC::TextRenderComponent* m_pP2Letter{ nullptr };

		std::string m_SafeFilePath{ "Data/Highscores.txt" };
		std::string m_FileContent{};
	};
}