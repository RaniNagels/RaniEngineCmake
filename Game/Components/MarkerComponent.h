#pragma once
#include <Components/Component.h>
#include <vector>
#include <glm/glm.hpp>

namespace Game
{
	struct MarkerDescriptor
	{
		std::vector<glm::vec2> positions;
	};

	class MarkerComponent : public REC::Component
	{
	public:
		explicit MarkerComponent(REC::GameObject* owner, const MarkerDescriptor& desc);
		virtual ~MarkerComponent() = default;

		MarkerComponent(const MarkerComponent&) = delete;
		MarkerComponent(MarkerComponent&&) noexcept = delete;
		MarkerComponent& operator=(const MarkerComponent&) = delete;
		MarkerComponent& operator=(MarkerComponent&&) noexcept = delete;

		virtual void Update(float deltaT) override;
		void MoveToNextPosition(bool moveUp);
		uint8_t GetCurrentIndex() const;

	private:
		glm::vec2 GetNextPosition(bool moveUp) const;

		MarkerDescriptor m_Descriptor;
		glm::vec2 m_CurrentPosition;
	};
}