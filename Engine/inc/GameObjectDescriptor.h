#pragma once
#include <cstdint>
#include <GeneralStructs.h>
#include <ComponentDescriptors.h>
#include <memory>

namespace REC
{
	enum class CollisionType : uint8_t
	{
		Static,
		Dynamic,
		NoCollision
	};

	struct GameObjectDescriptor
	{
		Rect bounds{}; // collision bounds
		//std::vector<std::unique_ptr<ComponentDescriptor>> components{}; 
		float startPosX{ 0 };
		float startPosY{ 0 };
		CollisionType collisionType{ CollisionType::NoCollision };
		uint8_t renderLayer{ 0 };
	};
}