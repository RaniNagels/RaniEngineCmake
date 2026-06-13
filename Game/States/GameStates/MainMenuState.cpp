#include "MainMenuState.h"
#include "../../RenderLayers.h"
#include <SceneManager.h>
#include <Events/Event.h>
#include <Input/InputSystem.h>
#include <Input/InputBinding.h>
#include <Util.h>
#include <GameObject.h>
#include <Components/SpriteRenderComponent.h>
#include <Components/TextRenderComponent.h>
#include "../../Ids.h"
#include "PlayerMenuState.h"
#include <ServiceLocator.h>

Game::MainMenuState::MainMenuState(const REC::EngineContext& context)
	: REC::GameState(context)
{}

void Game::MainMenuState::Enter()
{
	REC::Scene* scene = CreateScene(Game::SceneIds::EntryScene);

	m_pStartGameInputBinding = GetContext().inputSystem->CreateInputBinding();
	m_pStartGameInputBinding->AddInputAction<REC::KeyboardButtonAction>(REC::Input::Keyboard::Button::Keyboard_Return, REC::ButtonState::Up);
	m_pStartGameInputBinding->AddInputAction<REC::ControllerButtonAction>(REC::Input::Controller::Button::Gamepad_A, REC::ButtonState::Up);
	m_pStartGameInputBinding->AddEvent<REC::Event>(EventIds::StartGameEvent);

	SubscribeToEvent({ EventIds::StartGameEvent });

	REC::SpriteDescriptor startScreenBackdrop{};
	startScreenBackdrop.drawHeight = 750;
	startScreenBackdrop.textureKey = "titleScreen";
	startScreenBackdrop.frameDataFileKey = "startScreenData";
	startScreenBackdrop.frameKey = "start_up_screen_1987";

	REC::GameObjectDescriptor startScreenBackdropDesc{};
	startScreenBackdropDesc.startPosX = 350.f;
	startScreenBackdropDesc.startPosY = 0.f;
	startScreenBackdropDesc.renderLayer = Util::to_underlying(RenderLayer::Background); // C++23 feature

	auto* backdrop = scene->CreateGameObject(startScreenBackdropDesc);
	backdrop->AddComponent<REC::SpriteRenderComponent>(startScreenBackdrop);

	// marker
	REC::GameObjectDescriptor markerDesc{};
	markerDesc.id = Game::ObjectIds::Marker;
	markerDesc.renderLayer = Util::to_underlying(Game::RenderLayer::Player);
	markerDesc.startPosX = 200.f;
	markerDesc.startPosY = 475.f;
	markerDesc.parent = backdrop;

	REC::SpriteDescriptor markerSpriteDesc{};
	markerSpriteDesc.drawHeight = 25;
	markerSpriteDesc.drawWidth = 25;
	markerSpriteDesc.textureKey = "titleScreen";
	markerSpriteDesc.frameDataFileKey = "startScreenData";
	markerSpriteDesc.frameKey = ">_gray";
	markerSpriteDesc.drawPointX = 0.f;
	markerSpriteDesc.drawPointY = 0.f;

	auto* marker = scene->CreateGameObject(markerDesc);
	marker->AddComponent<REC::SpriteRenderComponent>(markerSpriteDesc);

	auto& SS = REC::ServiceLocator::GetSoundSystem();
	SS.Play("BackgroundMusic", 0.5f);
}

std::optional<std::unique_ptr<REC::GameState>> Game::MainMenuState::OnEvent(REC::Event* event)
{
	if (event->IsEvent(Game::EventIds::StartGameEvent))
	{
		auto& SS = REC::ServiceLocator::GetSoundSystem();
		SS.Play("stepHorizontalSound", 1.f);
		return std::make_unique<PlayerMenuState>(GetContext());
	}
	return {};
}

void Game::MainMenuState::Exit()
{
	UnsubscribeFromEvent({ Game::EventIds::StartGameEvent });
	GetContext().inputSystem->RemoveInputBinding(m_pStartGameInputBinding);
	GetScene()->RemoveAll();
}