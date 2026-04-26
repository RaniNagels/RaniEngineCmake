#pragma once
#include <cstdint>
#include <GeneralStructs.h>
#include <ComponentDescriptors.h>
#include <memory>
#include <GameObject.h>
#include <optional>

namespace REC
{
	using ObjectId = unsigned int;

	struct GameObjectDescriptor
	{
		std::optional<ObjectId> id;
		//std::vector<std::unique_ptr<ComponentDescriptor>> components{}; 
		float startPosX{ 0 }; // relative to parent, absolute if no parent
		float startPosY{ 0 }; // relative to parent, absolute if no parent
		uint8_t renderLayer{ 0 };

		GameObject* parent{ nullptr };
	};
}