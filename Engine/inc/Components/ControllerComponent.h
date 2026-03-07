#pragma once
#include <Components/Component.h>
#include <cstdint>

namespace REC
{
	// TODO: temporary -> make an attribute Component
	class ControllerComponent final : public Component
	{
	public:
		ControllerComponent(GameObject* owner, uint8_t controllerID)
			: Component(owner), m_ControllerIndex{controllerID} { }
		virtual ~ControllerComponent() = default;

		ControllerComponent(const ControllerComponent& other) = delete;
		ControllerComponent(ControllerComponent&& other) = delete;
		ControllerComponent& operator=(const ControllerComponent& other) = delete;
		ControllerComponent& operator=(ControllerComponent&& other) = delete;

		virtual void Update(float) override {}

		bool MatchId(uint8_t ID) { return m_ControllerIndex == ID; }

	private:
		uint8_t m_ControllerIndex;
	};
}