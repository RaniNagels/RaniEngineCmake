#pragma once
#include <Components/Component.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <GeneralStructs.h>

namespace Game
{
	struct GridDescriptor final
	{
		uint8_t rows;
		uint8_t cols;

		uint8_t cellWidth;
		uint8_t cellHeight;

		glm::vec2 origin;
	};

	class GridComponent final : public REC::Component
	{
	public:
		explicit GridComponent(REC::GameObject* owner, const GridDescriptor& desc);
		virtual ~GridComponent() = default;

		GridComponent(const GridComponent& other) = delete;
		GridComponent(GridComponent&& other) = delete;
		GridComponent& operator=(const GridComponent& other) = delete;
		GridComponent& operator=(GridComponent&& other) = delete;

		virtual void Update(float deltaT) override;

		struct Cell
		{
			glm::vec2 origin{}; // must be local!

			uint8_t width{};
			uint8_t height{};

			uint8_t row{uint8_t(-1)};
			uint8_t col{uint8_t(-1)};

			bool isWall{ false };

			bool operator==(const Cell& other) const
			{
				 return row == other.row && col == other.col && origin == other.origin && isWall == other.isWall && width == other.width && height == other.height;
			}

			bool operator!=(const Cell& other) const
			{
				return !(*this == other);
			}

			REC::Rect GetRect() const
			{
				return REC::Rect{ origin.x, origin.y, float(width), float(height) };
			}

			bool IsInCell(const glm::vec2& pos) const
			{
				return pos.x >= origin.x && pos.x < origin.x + width
					&& pos.y >= origin.y && pos.y < origin.y + height;
			}

			bool IsValid() const
			{
				return row != uint8_t(-1) && col != uint8_t(-1);
			}
		};

		const std::vector<Cell>& GetCells() const { return m_Cells; }
		const Cell& GetCell(uint8_t row, uint8_t col);
		const Cell& GetCell(const glm::vec2& pos);
		glm::vec2 GetAbsoluteCellPosition(const Cell& cell); // returns the world position!, the cell only contains the local position in relation of the grid
		glm::vec2 GetAbsoluteCellPosition(uint8_t row, uint8_t col) { return GetAbsoluteCellPosition(GetCell(row, col)); }
		glm::vec2 GetAbsoluteCellPosition(const glm::vec2& pos) { return GetAbsoluteCellPosition(GetCell(pos)); }

		std::vector<REC::CollisionBound> GetWallCollisionBounds();

	private:

		GridDescriptor m_Descriptor;
		std::vector<Cell> m_Cells;
		static Cell s_InvalidCell;

		uint32_t GetIndex(Cell* cell);
		uint32_t GetIndex(uint8_t row, uint8_t col);
	};
}