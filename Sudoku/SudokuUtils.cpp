#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "boost/container/static_vector.hpp"
#include <algorithm>

using namespace Sudoku;

namespace str = std::ranges;

Index SudokuUtils::countOccurence(const CandidatesGrid &grid, std::span<const Cell> cells, Value value)
{
   return str::count_if(cells, [&grid, value](Cell cell) { return grid[cell.field].contains(value); });
}

boost::container::static_vector<Value, NumValue> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid,
                                                                                           std::span<const Cell> cells)
{
   ValueBits activeValues;
   for (Cell cell : cells)
   {
      for (Value value : grid[cell.field])
      {
         activeValues[value] = true;
      }
   }

   const auto valueIsSolved = SudokuUtils::getSolvedValues(grid, cells);
   boost::container::static_vector<Value, NumValue> result;
   for (Value value : ValueAll)
   {
      if (activeValues[value] && !valueIsSolved.test(value))
      {
         result.push_back(value);
      }
   }
   return result;
}

FieldBits SudokuUtils::getNotSolvedCells(const CandidatesGrid &grid, std::span<const Cell> cells)
{
   FieldBits result;
   for (const auto &c : cells)
   {
      if (!grid[c.field].isSolved())
      {
         result.set(c.field);
      }
   }
   return result;
}

FieldBits SudokuUtils::getNotSolvedCells(const CandidatesGrid &grid)
{
   return getNotSolvedCells(grid, Cell::getAll());
}

FieldBits SudokuUtils::getSolvedCells(const CandidatesGrid &grid)
{
   FieldBits result;
   for (Field field : FieldAll)
   {
      if (grid[field].isSolved())
      {
         result.set(field);
      }
   }
   return result;
}

ValueBits SudokuUtils::getSolvedValues(const CandidatesGrid &grid, std::span<const Cell> cells)
{
   ValueBits result;
   for (auto cell : cells)
   {
      const auto &candidates = grid[cell.field];
      const auto solved = candidates.tryGetSolvedValue();
      if (solved.has_value())
      {
         result.set(solved.value());
      }
   }
   return result;
}

ValueBits SudokuUtils::getSolvedValues(const CandidatesGrid &grid, std::span<const Field> fields)
{
   ValueBits result;
   for (auto field : fields)
   {
      const auto &candidates = grid[field];
      const auto solved = candidates.tryGetSolvedValue();
      if (solved.has_value())
      {
         result.set(solved.value());
      }
   }
   return result;
}

SudokuUtils::Value2Fields SudokuUtils::getValue2Fields(const CandidatesGrid &grid, std::span<Field> fields)
{
   Value2Fields result;
   for (auto field : fields)
   {
      for (auto value : grid[field])
      {
         result[value].push_back(field);
      }
   }
   return result;
}

SudokuUtils::Value2Fields SudokuUtils::getValue2Fields(const CandidatesGrid &grid, std::span<const Cell> cells)
{
   boost::container::static_vector<Field, NumField> fields(cells.size());
   str::transform(cells, fields.begin(), Cell::getField);
   return getValue2Fields(grid, fields);
}
