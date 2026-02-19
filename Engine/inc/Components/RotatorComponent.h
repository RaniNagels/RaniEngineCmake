#pragma once
#include "Component.h"

namespace REC
{
	class TransformComponent;
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* owner, float speed = 1.f);
		virtual ~RotatorComponent() = default;

		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		virtual void Update(float deltaT) override;

	private:
		TransformComponent* m_pTransform;
		float m_RotationSpeed; // radians per second
	};
}