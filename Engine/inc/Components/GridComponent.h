#pragma once
#include "Component.h"
#include <cstdint>
#include <glm/glm.hpp>

namespace REC
{
	struct GridDescriptor final
	{
		uint8_t rows;
		uint8_t cols;

		uint8_t cellWidth;
		uint8_t cellHeight;

		glm::vec2 origin;
	};

	class GridComponent final : public Component
	{
	public:
		explicit GridComponent(GameObject* owner, const GridDescriptor& desc);
		virtual ~GridComponent() = default;

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		virtual void Update(float deltaT) override;

		struct Cell
		{
			glm::vec2 origin{};
			uint8_t row{};
			uint8_t col{};
		};

		const GridDescriptor& GetDescriptorData() const { return m_Descriptor; }
		const std::vector<Cell>& GetCells() const { return m_Cells; }

	private:

		GridDescriptor m_Descriptor;
		std::vector<Cell> m_Cells;

		uint32_t GetIndex(Cell* cell);
		uint32_t GetIndex(uint8_t row, uint8_t col);
	};
}