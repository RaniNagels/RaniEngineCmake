#include "SaveScoreState.h"

Game::SaveScoreState::SaveScoreState(const REC::EngineContext& context)
	: REC::GameState(context)
{}

void Game::SaveScoreState::Enter()
{}

std::optional<std::unique_ptr<REC::GameState>> Game::SaveScoreState::OnEvent(REC::Event*)
{
	return {};
}

void Game::SaveScoreState::Exit()
{}