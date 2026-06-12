#pragma once
#include <Components/Component.h>
#include <cstdInt>

namespace Game
{
	class PickUpComponent final : public REC::Component
	{
	public:
		enum class PickUpType
		{
			ExtraBomb, 
			Detonator,
			Flames
		};

		explicit PickUpComponent(REC::GameObject* owner, uint8_t type);
		virtual ~PickUpComponent();

		PickUpComponent(const PickUpComponent&) = delete;
		PickUpComponent(PickUpComponent&&) = delete;
		PickUpComponent& operator= (const PickUpComponent&) = delete;
		PickUpComponent& operator= (PickUpComponent&&) = delete;

		virtual void Update(float deltaT) override;

		bool IsType(Game::PickUpComponent::PickUpType type) const { return m_Type == type; }

	private:
		PickUpType m_Type;
	};
}