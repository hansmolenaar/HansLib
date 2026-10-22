#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "UniqueSorted.h"
#include <algorithm>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

CandidatesInHouse::CandidatesInHouse(const CandidatesGrid &cg, House house) : m_grid(&cg), m_house(std::move(house))
{
   for (auto cell : m_house.cells)
   {
      const Field field = cell.field;
      const auto numCandidates = cg[field].size();
      m_multiplicities.at(numCandidates).push_back(field);
      for (auto value : ValueAll)
      {
         if (cg[field].contains(value))
         {
            m_fieldsContainingValue[value].push_back(field);
         }
      }
   }
}

const House &CandidatesInHouse::getHouse() const
{
   return m_house;
}

std::span<const Field> CandidatesInHouse::getFieldsContainingValue(Value value) const
{
   return m_fieldsContainingValue.at(value);
}

const CandidatesGrid &CandidatesInHouse::getGrid() const
{
   return *m_grid;
}

Index SudokuUtils::countOccurence(const CandidatesGrid &grid, std::span<const Cell> cells, Value value)
{
   return str::count_if(cells, [&grid, value](Cell cell) { return grid[cell.field].contains(value); });
}

std::set<Value> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid, std::span<const Field> fields)
{
   std::set<Value> result;
   boost::container::static_vector<Value, NumIndex> solvedValues;
   for (Field field : fields)
   {
      const auto &candidates = grid[field];
      // Store the known values
      if (candidates.isSolved())
      {
         solvedValues.push_back(candidates.tryGetSolvedValue().value());
      }
      else
      {
         for (Value value : ValueAll)
         {
            if (candidates.contains(value))
            {
               result.insert(value);
            }
         }
      }
   }
   for (Value value : solvedValues)
   {
      result.erase(value);
   }

   return result;
}

std::set<Value> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid, std::span<const Cell> cells)
{
   std::vector<Field> fields(cells.size());
   str::transform(cells, fields.begin(), [](Cell cell) { return cell.field; });
   return getUniqueActiveValuesInCells(grid, fields);
}

std::span<const Field> CandidatesInHouse::getFieldsWithMultiplicity(Index multiplicity) const
{
   return m_multiplicities.at(multiplicity);
}

Index CandidatesInHouse::getNumCandidatesInCell(Field field) const
{
   return (*m_grid)[field].size();
}

boost::container::static_vector<Cell, NumCellsInHouse> SudokuUtils::getNotSolvedCells(const CandidatesGrid &grid,
                                                                                      const House &house)
{
   boost::container::static_vector<Cell, NumCellsInHouse> result(NumCellsInHouse);
   const auto last = std::copy_if(house.cells.begin(), house.cells.end(), result.begin(),
                                  [&grid](Cell cell) { return !grid[cell.field].isSolved(); });
   result.erase(last, result.end());
   return result;
}
