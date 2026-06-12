#pragma once
#include <Components/Component.h>
#include <cstdint>
#include <glm/glm.hpp>
#include <GeneralStructs.h>
#include <LevelInfo.h>
#include <Events/Event.h>
#include <EngineContext.h>

namespace Game
{
	struct GridEventArgs final : public REC::EventArgs
	{
		uint8_t row{};
		uint8_t col{};
		REC::GameObject* grid{ nullptr };

		virtual std::unique_ptr<REC::EventArgs> makeUnique() const override
		{
			return std::make_unique<GridEventArgs>(*this);
		}
		virtual ~GridEventArgs() = default;
	};

	struct GridDescriptor final
	{
		uint8_t rows;
		uint8_t cols;

		uint8_t cellWidth;
		uint8_t cellHeight;

		glm::vec2 origin;
		REC::LevelInfo* levelInfo;
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
			bool isDestructableWall{ false };
			bool hasExit{ false };
			bool hasPowerUp{ false };

			uint8_t powerUpType{ uint8_t(-1) }; // only valid if hasPowerUp is true

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
		void GetModifiableCell(uint8_t row, uint8_t col, Cell& cell);
		glm::vec2 GetAbsoluteCellPosition(const Cell& cell); // returns the world position!, the cell only contains the local position in relation of the grid
		glm::vec2 GetAbsoluteCellPosition(uint8_t row, uint8_t col) { return GetAbsoluteCellPosition(GetCell(row, col)); }
		glm::vec2 GetAbsoluteCellPosition(const glm::vec2& pos) { return GetAbsoluteCellPosition(GetCell(pos)); }

		std::vector<REC::CollisionBound> GetWallCollisionBounds();
		glm::vec2 GetCellSize() const { return glm::vec2{ m_Descriptor.cellWidth, m_Descriptor.cellHeight }; }
		uint8_t GetRows() const { return m_Descriptor.rows; }
		uint8_t GetCols() const { return m_Descriptor.cols; }
		bool IsRowColValid(uint8_t row, uint8_t col) const { return row < m_Descriptor.rows && col < m_Descriptor.cols; }

		void SetNewLevel(REC::LevelInfo* levelInfo);
		REC::LevelInfo* GetLevelInfo() const { return m_pLevelInfo; }
		bool LoadNextLevel(const REC::EngineContext& context);
		void ResetGrid();

		void ModifyCell(uint8_t row, uint8_t col, bool isDestructableWall, bool hasExit = false, bool hasPowerUp = false);

	private:

		GridDescriptor m_Descriptor;
		REC::LevelInfo* m_pLevelInfo;
		std::vector<Cell> m_Cells;
		static Cell s_InvalidCell;

		uint32_t GetIndex(Cell* cell);
		uint32_t GetIndex(uint8_t row, uint8_t col);
		
		std::unique_ptr<REC::Event> m_pLevelChangeEvent{ nullptr };
	};
}