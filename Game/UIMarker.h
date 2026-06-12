#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace REC
{
	class GameObject;
	class Scene;
	class InputBinding;
	class InputSystem;
}

namespace Game
{
	class MarkerComponent;

	struct UIMarkerDescriptor
	{
		std::vector<glm::vec2> positions{};
		REC::Scene* scene{ nullptr };
		REC::GameObject* root{ nullptr };
		glm::vec2 drawSize{ 20.f, 20.f };
	};

	class UIMarker final
	{
	public:
		explicit UIMarker(const UIMarkerDescriptor& descriptor);
		~UIMarker() = default;

		UIMarker(const UIMarker&) = delete;
		UIMarker(UIMarker&&) noexcept = delete;
		UIMarker& operator=(const UIMarker&) = delete;
		UIMarker& operator=(UIMarker&&) noexcept = delete;

		REC::GameObject* Get() const { return m_pMarkerGO; }
		void CreateInputBindings(REC::InputSystem* inputSystem);
		void DeleteInputBindings(REC::InputSystem* inputSystem);
		uint8_t GetCurrentIndex() const;

	private:
		REC::GameObject* m_pMarkerGO{ nullptr };
		MarkerComponent* m_pMarkerComponent{ nullptr };
		REC::Scene* m_pScene{ nullptr };
		std::vector<REC::InputBinding*> m_InputBindings{};
	};
}