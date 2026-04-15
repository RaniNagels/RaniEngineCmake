#include "GridComponent.h"
#include <GameObject.h>
#include <Components/TransformComponent.h>

Game::GridComponent::Cell Game::GridComponent::s_InvalidCell{};

Game::GridComponent::GridComponent(REC::GameObject* owner, const GridDescriptor& desc)
	: Component(owner)
	, m_Descriptor{desc}
	, m_Cells{}
{
	uint32_t amountOfCells{ uint32_t(desc.cols) * uint32_t(desc.rows) };
	m_Cells.resize(amountOfCells);

	auto worldPos = GetOwner()->GetTransform()->GetWorldPosition();

	for (uint8_t r{}; r < desc.rows; ++r)
	{
		for (uint8_t c{}; c < desc.cols; ++c)
		{
			auto index = GetIndex(r, c);
			m_Cells[index].width = desc.cellWidth;
			m_Cells[index].height = desc.cellHeight;
			m_Cells[index].row = r;
			m_Cells[index].col = c;
			m_Cells[index].origin = { (worldPos.x + (c * desc.cellWidth)), (worldPos.y + (r * desc.cellHeight)) };
			if (r == 0 || r == desc.rows - 1 || c == 0 || c == desc.cols - 1 || (r %2 == 0 && c % 2 == 0))
				m_Cells[index].isWall = true;
		}
	}
}

void Game::GridComponent::Update(float) {}

const Game::GridComponent::Cell& Game::GridComponent::GetCell(uint8_t row, uint8_t col)
{
	return m_Cells[GetIndex(row, col)];
}

const Game::GridComponent::Cell& Game::GridComponent::GetCell(const glm::vec2& pos)
{
	for (const auto& cell : m_Cells)
	{
		if (cell.IsInCell(pos))
			return cell;
	}
	return s_InvalidCell;
}

uint32_t Game::GridComponent::GetIndex(Cell* cell)
{
	return uint16_t(m_Descriptor.cols) * uint16_t(cell->row) + uint16_t(cell->col);
}

uint32_t Game::GridComponent::GetIndex(uint8_t row, uint8_t col)
{
	return uint16_t(m_Descriptor.cols) * uint16_t(row) + uint16_t(col);
}