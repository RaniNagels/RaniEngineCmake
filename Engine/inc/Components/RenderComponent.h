#pragma once
#include "Component.h"

namespace REC
{
	// a component class that can render something,
	// should be used as a base class for all render components
	// should not be used directly!
	class RenderComponent : public Component
	{
	protected:
		// enforce that only derived classes can create components!
		explicit RenderComponent(GameObject* owner) : Component(owner) {}

	public:
		virtual ~RenderComponent() = default;

		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Render() = 0;
		virtual bool CanRender() const override { return true; }

	};
}