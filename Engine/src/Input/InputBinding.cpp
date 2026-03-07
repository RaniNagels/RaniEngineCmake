#include <Input/InputBinding.h>

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

void REC::InputBinding::Execute(uint8_t controllerIndex, float inputStrenght) const
{
	for (auto& command : m_Commands)
		command->Execute(controllerIndex, inputStrenght);
}
