#pragma once
#include <sdbm_hash.h>
#include <GameObject.h>
#include <Events/Event.h>
#include <Scene.h>

namespace Game
{
	namespace EventIds
	{
		// add game event ids here...
		constexpr REC::EventId MoveEvent = REC::make_sdbm_hash("MoveEvent");
		constexpr REC::EventId HasPlaceBombEvent = REC::make_sdbm_hash("HasPlacedBombEvent");
		constexpr REC::EventId BombermanDeathEvent = REC::make_sdbm_hash("BombermanDeathEvent");
		constexpr REC::EventId DoorOpenEvent = REC::make_sdbm_hash("DoorOpenEvent");
		constexpr REC::EventId VeryDeathEvent = REC::make_sdbm_hash("VeryDeathEvent");

		// game state events
		constexpr REC::EventId StartGameEvent = REC::make_sdbm_hash("StartGameEvent");
		constexpr REC::EventId MarkerSelectedEvent = REC::make_sdbm_hash("MarkerSelectedEvent");
	}

	namespace ObjectIds
	{
		constexpr REC::ObjectId Grid = REC::make_sdbm_hash("Grid");
		constexpr REC::ObjectId Bomberman = REC::make_sdbm_hash("Bomberman");
		constexpr REC::ObjectId Balloom = REC::make_sdbm_hash("Balloom");
		constexpr REC::ObjectId Bom = REC::make_sdbm_hash("Bom");
		constexpr REC::ObjectId PickUp = REC::make_sdbm_hash("PickUp");

		// UI elements
		constexpr REC::ObjectId Player1UI = REC::make_sdbm_hash("Player1UI");
		constexpr REC::ObjectId Player2UI = REC::make_sdbm_hash("Player2UI");
		constexpr REC::ObjectId GameOverInstructions = REC::make_sdbm_hash("GameOverInstructions");
		constexpr REC::ObjectId Marker = REC::make_sdbm_hash("Marker");
	}

	namespace SceneIds
	{
		constexpr REC::SceneId EntryScene = REC::make_sdbm_hash("StartUpScene");
		constexpr REC::SceneId LevelScene = REC::make_sdbm_hash("LevelScene");
		constexpr REC::SceneId GameOverScene = REC::make_sdbm_hash("GameOverScene");
	}
}