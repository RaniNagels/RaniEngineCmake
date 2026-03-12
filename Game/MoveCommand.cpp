#include "MoveCommand.h"
#include <Components/TransformComponent.h>
#include <Components/ControllerComponent.h>

Game::MoveCommand::MoveCommand(REC::GameObject* actor, glm::vec2 direction, float speed)
	: GameObjectInputCommand(actor)
	, m_Direction{direction}
	, m_Speed{speed}
    , m_pControllerComponent{ GetGameObject()->GetComponent<REC::ControllerComponent>()}
{
    m_Direction.x = std::clamp(m_Direction.x, -1.f, 1.f);
    m_Direction.y = std::clamp(m_Direction.y, -1.f, 1.f);
}

void Game::MoveCommand::Execute(float deltaTime)
{
    // needs to be executed when: controllerIndex == -1 OR if controllerIndex != -1 AND controllerIndex == ControllerComponent.id
    if (GetControllerIndex() != static_cast<uint8_t>(-1))
    {
        if (!m_pControllerComponent || !m_pControllerComponent->MatchId(GetControllerIndex()))
            return;
    }

    glm::vec3 movement{ m_Direction, 0.f };
    movement *= m_Speed * deltaTime;

    if (GetControllerIndex() != static_cast<uint8_t>(-1))
        movement *= GetInputStrength();

    GetGameObject()->GetTransform()->AddToLocalPosition(movement);
}
