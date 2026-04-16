#pragma once
#include <ComponentDescriptors.h>
#include <glm/glm.hpp>
#include <Input/Input.h>
#include <Input/InputAction.h>
#include <Input/InputBinding.h>
#include <memory>

namespace REC
{
	class GameObject;
	class Scene;
	class SceneManager;
	class InputSystem;
	class EventSystem;

	class SpriteRenderComponent;
	class AnimatedSpriteComponent;
	class HealthComponent;
	class LivesComponent;
}

namespace Game
{
	class PlaceBombCommand;
	class GridComponent;

	struct PlayerDescriptor
	{
		REC::SpriteDescriptor spriteDesc{};
		REC::AnimationDescriptor animDesc{};

		int amountOfLives{ 0 };
		float maxHealth{ 0.f };

		uint8_t renderLayer{ 0 };
		glm::vec2 startPosition{};
	};

	// used for quick look ups during initialization
	// to avoid using GetComponent during init
	struct PlayerComponents
	{
		REC::AnimatedSpriteComponent* animComp{};
		REC::SpriteRenderComponent* spriteComp{};
		REC::HealthComponent* healthComp{};
		REC::LivesComponent* livesComp{};
	};

	struct PlayerCommands
	{
		PlaceBombCommand* placeBombCmd{};
	};

	
	template <typename action>
	concept ActionType = std::derived_from<action, REC::IInputAction>;

	template <ActionType AT>
	struct PlayerInputActions
	{
		std::unique_ptr<AT> up{};
		std::unique_ptr<AT> down{};
		std::unique_ptr<AT> left{};
		std::unique_ptr<AT> right{};
		std::unique_ptr<AT> placeBomb{};
		// extent the list
	};

	
	class Player final
	{
	public:
		explicit Player(REC::Scene* scene, const PlayerDescriptor& descriptor);
		~Player();

		Player(const Player& other) = delete;
		Player(Player&& other) = delete;
		Player& operator=(const Player& other) = delete;
		Player& operator=(Player&& other) = delete;

		const PlayerComponents& GetComponents() const { return m_Components; }
		const PlayerCommands& GetCommands() const { return m_Commands; }
		REC::GameObject* Get() const { return m_pGameObject; }

		void CreateInputBindings(REC::InputSystem* inputsystem, REC::SceneManager* sceneManager, float movementSpeed, GridComponent* playfield);

		template <ActionType AT>
		void AddInputActions(PlayerInputActions<AT>& inputActions) // inputActions cannot be const due to the move
		{
			if (inputActions.up.get() != nullptr)			m_InputBindings[InputBindingIndex::Up]->AddInputAction(std::move(inputActions.up));
			if (inputActions.down.get() != nullptr)			m_InputBindings[InputBindingIndex::Down]->AddInputAction(std::move(inputActions.down));
			if (inputActions.left.get() != nullptr)			m_InputBindings[InputBindingIndex::Left]->AddInputAction(std::move(inputActions.left));
			if (inputActions.right.get() != nullptr)		m_InputBindings[InputBindingIndex::Right]->AddInputAction(std::move(inputActions.right));
			if (inputActions.placeBomb.get() != nullptr)	m_InputBindings[InputBindingIndex::PlaceBomb]->AddInputAction(std::move(inputActions.placeBomb));
		}

	private:
		// unscoped for ease of access
		// only used for indexing the input bindings
		// DO NOT USE THIS OUTSIDE OF THE PLAYER CLASS
		enum InputBindingIndex
		{
			Up,
			Down,
			Left,
			Right,
			PlaceBomb,
			// etc
			Count,
		};

		PlayerDescriptor m_Descriptor;
		PlayerComponents m_Components;
		PlayerCommands m_Commands;
		REC::GameObject* m_pGameObject;

		// up, down, left, right, place bomb, ...
		// use the same order as the PlayerInputActions struct
		std::vector<REC::InputBinding*> m_InputBindings{};
	};
}