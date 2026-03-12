#include <Input/InputBinding.h>
#include <Commands/ICommand.h>

std::vector<REC::IInputAction*> REC::InputBinding::GetInputActions(InputActionType type) const
{
	std::vector<IInputAction*> outActions{};
	for (auto& action : m_Actions)
	{
		if (action->GetType() == type)
			outActions.emplace_back(action.get());
	}
	return outActions;
}

void REC::InputBinding::Execute(float deltaTime, uint8_t controllerIndex, float inputStrenght) const
{
	for (auto& command : m_Commands)
	{
		if (auto GOIC = dynamic_cast<GameObjectInputCommand*>(command.get()))
		{
			GOIC->SetInputStrength(inputStrenght);
			GOIC->TriggeredByController(controllerIndex);
		}

		command->Execute(deltaTime);
	}
}
