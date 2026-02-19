#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace REC
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* owner);
		explicit TransformComponent(GameObject* owner, const glm::vec3& position);
		explicit TransformComponent(GameObject* owner, float x, float y, float z = 0.f);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void Update(float deltaT) override;

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);

	private:
		glm::vec3 m_Position{};
		bool m_IsWorldPositionValid{ false };
	};
}