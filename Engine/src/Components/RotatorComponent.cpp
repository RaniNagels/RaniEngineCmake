#include "../../inc/Components/RotatorComponent.h"
#include <glm/fwd.hpp>
#include "../../inc/GameObject.h"
#include "../../inc/Components/TransformComponent.h"

REC::RotatorComponent::RotatorComponent(GameObject* owner, float speed)
	: Component(owner)
	, m_pTransform{GetOwner()->GetTransform()}
	, m_RotationSpeed{speed}
{
}

void REC::RotatorComponent::Update(float deltaT)
{
	glm::vec2 localPos = m_pTransform->GetLocalPosition();
	float radius = glm::length(localPos);
	if (radius == 0) return;

	float currentAngle = std::atan2(localPos.y, localPos.x);
	currentAngle += m_RotationSpeed * deltaT;

	float newX = std::cos(currentAngle) * radius;
	float newY = std::sin(currentAngle) * radius;

	m_pTransform->SetLocalPosition(newX, newY, m_pTransform->GetLocalPosition().z);
}
