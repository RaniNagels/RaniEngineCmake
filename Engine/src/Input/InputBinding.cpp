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

void REC::InputBinding::Execute(float deltaTime, float inputStrenght) const
{
	for (auto& command : m_Commands)
	{
		if (auto GOIC = dynamic_cast<GameObjectInputCommand*>(command.get()))
		{
			GOIC->SetStrength(inputStrenght);
		}

		command->Execute(deltaTime);
	}
}
