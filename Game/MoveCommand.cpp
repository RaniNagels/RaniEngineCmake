#include "MoveCommand.h"
#include <Components/TransformComponent.h>
#include <Components/ControllerComponent.h>

Game::MoveCommand::MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed)
	: GameObjectInputCommand(actor)
	, m_Direction{direction}
	, m_Speed{speed}
{
}

void Game::MoveCommand::Execute()
{
    // needs to be executed when: controllerIndex == -1 OR if controllerIndex != -1 AND controllerIndex == ControllerComponent.id
    if (GetControllerIndex() != static_cast<uint8_t>(-1))
    {
        auto* controller = GetGameObject()->GetComponent<REC::ControllerComponent>();
        if (!controller || !controller->MatchId(GetControllerIndex()))
            return;
    }

    glm::vec3 movement{ m_Direction, 0.f };
    movement *= m_Speed;

    if (GetControllerIndex() != static_cast<uint8_t>(-1))
        movement *= GetInputStrength();

    GetGameObject()->GetTransform()->AddToLocalPosition(movement);
}
