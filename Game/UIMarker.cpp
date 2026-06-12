#include "UIMarker.h"
#include <GameObject.h>
#include <Scene.h>
#include "Ids.h"
#include <Util.h>
#include "Components/MarkerComponent.h"
#include "RenderLayers.h"
#include <Input/InputBinding.h>
#include <Input/InputSystem.h>
#include <Components/SpriteRenderComponent.h>
#include "Commands/MoveMarkerCommand.h"

Game::UIMarker::UIMarker(const UIMarkerDescriptor& descriptor)
	: m_pScene(descriptor.scene)
{
	REC::GameObjectDescriptor markerDesc{};
	markerDesc.id = Game::ObjectIds::Marker;
	markerDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Player);
	markerDesc.startPosX = descriptor.positions[0].x;
	markerDesc.startPosY = descriptor.positions[0].y;
	markerDesc.parent = descriptor.root;

	REC::SpriteDescriptor markerSpriteDesc{};
	markerSpriteDesc.drawWidth = uint16_t(descriptor.drawSize.x);
	markerSpriteDesc.drawHeight = uint16_t(descriptor.drawSize.y);
	markerSpriteDesc.textureKey = "titleScreen";
	markerSpriteDesc.frameDataFileKey = "startScreenData";
	markerSpriteDesc.frameKey = ">_black";
	markerSpriteDesc.drawPointX = 0.f;
	markerSpriteDesc.drawPointY = 0.f;

	Game::MarkerDescriptor markerCompDesc{};
	markerCompDesc.positions = descriptor.positions;

	m_pMarkerGO = m_pScene->CreateGameObject(markerDesc);
	m_pMarkerComponent = m_pMarkerGO->AddComponent<Game::MarkerComponent>(markerCompDesc);
	m_pMarkerGO->AddComponent<REC::SpriteRenderComponent>(markerSpriteDesc);	
}

void Game::UIMarker::CreateInputBindings(REC::InputSystem* inputSystem)
{
	auto* moveUpMarker = inputSystem->CreateInputBinding();
	moveUpMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_W, REC::ButtonState::Up);
	moveUpMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Up, REC::ButtonState::Up);
	moveUpMarker->AddCommand<Game::MoveMarkerCommand>(m_pMarkerGO, true);

	auto* moveDownMarker = inputSystem->CreateInputBinding();
	moveDownMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_S, REC::ButtonState::Up);
	moveDownMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Down, REC::ButtonState::Up);
	moveDownMarker->AddCommand<Game::MoveMarkerCommand>(m_pMarkerGO, false);

	auto* selectMarker = inputSystem->CreateInputBinding();
	selectMarker->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Return, REC::ButtonState::Up);
	selectMarker->AddEvent<REC::Event>(Game::EventIds::MarkerSelectedEvent);

	m_InputBindings.push_back(moveUpMarker);
	m_InputBindings.push_back(moveDownMarker);
	m_InputBindings.push_back(selectMarker);
}

void Game::UIMarker::DeleteInputBindings(REC::InputSystem* inputSystem)
{
	for (auto& binding : m_InputBindings)
		inputSystem->RemoveInputBinding(binding);
}

uint8_t Game::UIMarker::GetCurrentIndex() const
{
	return m_pMarkerComponent->GetCurrentIndex();
}
