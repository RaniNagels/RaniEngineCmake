#include <ChangeSceneCommand.h>

REC::ChangeSceneCommand::ChangeSceneCommand(const EngineContext& context, Scene* toScene, Scene* fromScene)
	: ICommand(context)
	, m_ToScene{toScene}
	, m_FromScene{fromScene}
{
}

void REC::ChangeSceneCommand::Execute(uint8_t, float)
{
	auto* SM = GetContext()->sceneManager;
	if (m_FromScene == nullptr || SM->GetActiveScene() == m_FromScene)
	{
		SM->SetActiveScene(m_ToScene);
	}
}