#pragma once
#include <sdbm_hash.h>
#include <GameObject.h>
#include <Events/Event.h>

namespace Game
{
	namespace EventIds
	{
		// add game event ids here...
		constexpr REC::EventId MoveEvent = REC::make_sdbm_hash("MoveEvent");
		constexpr REC::EventId HasPlaceBombEvent = REC::make_sdbm_hash("HasPlacedBombEvent");
	}

	namespace ObjectIds
	{
		constexpr REC::ObjectId Grid = REC::make_sdbm_hash("Grid");
		constexpr REC::ObjectId Bomberman = REC::make_sdbm_hash("Bomberman");
		constexpr REC::ObjectId Balloom = REC::make_sdbm_hash("Balloom");
		constexpr REC::ObjectId Bom = REC::make_sdbm_hash("Bom");
	}
}