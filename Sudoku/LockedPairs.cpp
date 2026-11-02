#include "LockedPairs.h"
#include "House.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include <algorithm>
#include <ostream>

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

const Cell &LockedPair::operator[](int pos) const
{
   return m_cells.at(pos);
}

std::span<const Cell> LockedPair::getCells() const
{
   return m_cells;
}

std::ostream &operator<<(std::ostream &os, const Sudoku::LockedPair &lp)
{
   os << lp[0] << " <=> " << lp[1];
   return os;
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
            m_lockedPairs[value].emplace_back(value, Cell::get(fieldsWithValue[0]), Cell::get(fieldsWithValue[1]));
         }
      }
   }

   // Remove duplicates
   for (Value value : ValueAll)
   {
      UniqueSorted(m_lockedPairs.at(value));
   }
}
std::span<const LockedPair> LockedPairs::getPairs(Value value) const
{
   return m_lockedPairs.at(value);
}

std::ostream &operator<<(std::ostream &os, const Sudoku::LockedPairs &lps)
{
   os << "Locked Pairs\n\n";
   for (Value value : ValueAll)
   {
      const auto &pairs = lps.getPairs(value);
      for (const auto &lp : pairs)
      {
         os << value << " :  " << lp << "\n";
      }
   }
   return os;
}
