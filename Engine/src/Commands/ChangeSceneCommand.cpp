#include <Commands/ChangeSceneCommand.h>

REC::ChangeSceneCommand::ChangeSceneCommand(const EngineContext& context, Scene* toScene, Scene* fromScene)
	: EngineCommand(context)
	, m_ToScene{toScene}
	, m_FromScene{fromScene}
{
}

void REC::ChangeSceneCommand::Execute(float)
{
	auto* SM = GetContext()->sceneManager;
	if (m_FromScene == nullptr || SM->GetActiveScene() == m_FromScene)
	{
		SM->SetActiveScene(m_ToScene);
	}
}