#include "GridComponent.h"
#include <GameObject.h>
#include <Components/TransformComponent.h>
#include "../Ids.h"
#include <Resources/IResourceManager.h>

Game::GridComponent::Cell Game::GridComponent::s_InvalidCell{};

Game::GridComponent::GridComponent(REC::GameObject* owner, const GridDescriptor& desc)
	: Component(owner)
	, m_Descriptor{desc}
	, m_Cells{}
	, m_pLevelInfo{ desc.levelInfo }
{
	uint32_t amountOfCells{ uint32_t(desc.cols) * uint32_t(desc.rows) };
	m_Cells.resize(amountOfCells);

	//auto localPos = GetOwner()->GetTransform()->GetLocalPosition();

	for (uint8_t r{}; r < desc.rows; ++r)
	{
		for (uint8_t c{}; c < desc.cols; ++c)
		{
			auto index = GetIndex(r, c);
			m_Cells[index].width = desc.cellWidth;
			m_Cells[index].height = desc.cellHeight;
			m_Cells[index].row = r;
			m_Cells[index].col = c;
			m_Cells[index].origin = { (c * desc.cellWidth), (r * desc.cellHeight) };
			if (r == 0 || r == desc.rows - 1 || c == 0 || c == desc.cols - 1 || (r %2 == 0 && c % 2 == 0))
				m_Cells[index].isWall = true;
		}
	}

	m_pLevelChangeEvent = std::make_unique<REC::Event>(EventIds::LevelChangeEvent, REC::EventArgs{});
}

void Game::GridComponent::Update(float) {}

const Game::GridComponent::Cell& Game::GridComponent::GetCell(uint8_t row, uint8_t col)
{
	return m_Cells[GetIndex(row, col)];
}

const Game::GridComponent::Cell& Game::GridComponent::GetCell(const glm::vec2& pos)
{
	auto worldPos = GetOwner()->GetTransform()->GetWorldPosition();
	glm::vec2 worldPos2D{ worldPos.x, worldPos.y };
	glm::vec2 relativePos = pos - worldPos2D;

	for (const auto& cell : m_Cells)
	{
		if (cell.IsInCell(relativePos))
			return cell;
	}
	return s_InvalidCell;
}

void Game::GridComponent::GetModifiableCell(uint8_t row, uint8_t col, Cell& cell)
{
	cell = m_Cells[GetIndex(row, col)];
}

glm::vec2 Game::GridComponent::GetAbsoluteCellPosition(const Cell& cell)
{
	glm::vec2 worldPos = GetOwner()->GetTransform()->GetWorldPosition();
	glm::vec2 absolutePos = cell.origin + worldPos;
	return absolutePos;
}

std::vector<REC::CollisionBound> Game::GridComponent::GetWallCollisionBounds()
{
	std::vector<REC::CollisionBound> bounds{};

	for (const auto& cell : m_Cells)
	{
		if (cell.isWall)
			bounds.emplace_back(cell.GetRect(), false);
	}

	return bounds;
}

void Game::GridComponent::SetNewLevel(REC::LevelInfo* levelInfo)
{
	m_pLevelInfo = levelInfo;
	m_pLevelChangeEvent->Broadcast();
}

bool Game::GridComponent::LoadNextLevel(const REC::EngineContext& context)
{
	auto index = m_pLevelInfo->id + 1;
	if (index > 3) return false;
	SetNewLevel(context.resourceManager->GetLevelInfo(std::to_string(index)));
	return true;
}

void Game::GridComponent::ResetGrid()
{
	for (auto& cell : m_Cells)
	{
		cell.isDestructableWall = false;
		cell.hasExit = false;
		cell.hasPowerUp = false;
	}
}

void Game::GridComponent::ModifyCell(uint8_t row, uint8_t col, bool isDestructableWall, bool hasExit, bool hasPowerUp, uint8_t pickupType)
{
	uint32_t index = GetIndex(row, col);
	m_Cells[index].isDestructableWall = isDestructableWall;

	if (hasExit)
		m_Cells[index].hasExit = true;

	if (hasPowerUp)
	{
		m_Cells[index].hasPowerUp = true;
		m_Cells[index].powerUpType = pickupType;
	}
}

uint32_t Game::GridComponent::GetIndex(Cell* cell)
{
	return uint16_t(m_Descriptor.cols) * uint16_t(cell->row) + uint16_t(cell->col);
}

uint32_t Game::GridComponent::GetIndex(uint8_t row, uint8_t col)
{
	return uint16_t(m_Descriptor.cols) * uint16_t(row) + uint16_t(col);
}