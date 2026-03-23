#pragma once
#include <vector>
#include "Singleton.h"

namespace dae
{
    enum class TileType
    {
        Empty = 0,
        Platform = 1,
        Ladder = 2,
        Intersection = 3 
    };

    class LevelGrid final: public Singleton<LevelGrid>
    {
    public:
        void Initialize(int cols, int rows, float tileSize)
        {
            m_Cols = cols;
            m_Rows = rows;
            m_TileSize = tileSize;
            m_Grid.resize(cols * rows, TileType::Empty);
        }

        void SetTile(int x, int y, TileType type)
        {
            if (x >= 0 && x < m_Cols && y >= 0 && y < m_Rows)
                m_Grid[y * m_Cols + x] = type;
        }

        TileType GetTile(int x, int y) const
        {
            if (x >= 0 && x < m_Cols && y >= 0 && y < m_Rows)
                return m_Grid[y * m_Cols + x];
            return TileType::Empty;
        }

        float GetTileSize() const { return m_TileSize; }

        int GetCols() const { return m_Cols; }
        int GetRows() const { return m_Rows; }

    private:


        int m_Cols{ 0 }, m_Rows{ 0 };
        float m_TileSize{ 16.0f };
        std::vector<TileType> m_Grid;
    };
}