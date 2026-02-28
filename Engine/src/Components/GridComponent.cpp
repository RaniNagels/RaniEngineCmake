#include "../../inc/Components/GridComponent.h"

REC::GridComponent::GridComponent(GameObject* owner, const GridDescriptor& desc)
	: Component(owner)
	, m_Descriptor{desc}
	, m_Cells{}
{
	uint32_t amountOfCells{ uint32_t(desc.cols) * uint32_t(desc.rows) };
	m_Cells.resize(amountOfCells);

	for (uint8_t r{}; r < desc.rows; ++r)
	{
		for (uint8_t c{}; c < desc.cols; ++c)
		{
			auto index = GetIndex(r, c);
			m_Cells[index].row = r;
			m_Cells[index].col = c;
			m_Cells[index].origin = { (c * desc.cellWidth), (r * desc.cellHeight) };
		}
	}
}

void REC::GridComponent::Update(float) {}

uint32_t REC::GridComponent::GetIndex(Cell* cell)
{
	return uint16_t(m_Descriptor.cols) * uint16_t(cell->row) + uint16_t(cell->col);
}

uint32_t REC::GridComponent::GetIndex(uint8_t row, uint8_t col)
{
	return uint16_t(m_Descriptor.cols) * uint16_t(row) + uint16_t(col);
}