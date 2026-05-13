#pragma once
#include <glm/glm.hpp>

#include <Components/Component.h>

namespace REC
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent(GameObject* owner);
		explicit TransformComponent(GameObject* owner, float x, float y);
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void Update(float deltaT) override; // left empty

		glm::vec3 GetLocalPosition() const { return m_Position; }
		glm::vec3 GetWorldPosition();

		// local position!
		void AddToLocalPosition(float x, float y, float z = 0);
		void AddToLocalPosition(const glm::vec3& position);
		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& position);

		void RequiresUpdate();

		void SetStatic() { m_IsMoveable = false; }

	private:
		glm::vec3 m_Position{};
		//glm::vec3 m_Rotation{};

		glm::vec3 m_WorldPosition{};
		bool m_NeedsUpdate{ true };
		bool m_IsMoveable{ true };
	};
}