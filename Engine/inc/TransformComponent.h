#pragma once
#include "Component.h"

namespace REC
{
	class TransformComponent final : public Component
	{
	public:
		explicit TransformComponent() = default;
		virtual ~TransformComponent() = default;

		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void Update(float deltaT) override {}
	};
}