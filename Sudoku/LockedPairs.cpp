#include "LockedPairs.h"
#include "House.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include <algorithm>

using namespace Sudoku;
using namespace Utilities;

// Locked Pair

LockedPair::LockedPair(Value value, const Cell &cell0, const Cell &cell1) : m_value(value), m_cells{cell0, cell1}
{
   // Order the cells
   if (m_cells[0] > m_cells[1])
   {
      std::swap(m_cells[0], m_cells[1]);
   }
}

Value LockedPair::getValue() const
{
   return m_value;
}

std::span<const Cell> LockedPair::getCells() const
{
   return m_cells;
}

// Locked Pairs

LockedPairs::LockedPairs(const CandidatesGrid &grid)
{
   for (const auto &house : House::getAllHouses())
   {
      const SudokuUtils::Value2Fields v2f = SudokuUtils::getValue2Fields(grid, house.cells);
      for (Value value : ValueAll)
      {
         const auto &fieldsWithValue = v2f.at(value);
         if (fieldsWithValue.size() == 2)
         {
            m_lockedPairs.emplace_back(value, Cell::get(fieldsWithValue[0]), Cell::get(fieldsWithValue[1]));
         }
      }
   }
   // Remove duplicates
   UniqueSorted(m_lockedPairs);
}
