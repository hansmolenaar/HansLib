#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "MyException.h"
#include "boost/container/static_vector.hpp"
#include <algorithm>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

ValueBits SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid, const FieldBits &fields)
{
   ValueBits activeValues;
   for (Field field : FieldBitsToRange(fields))
   {
      for (Value value : grid[field])
      {
         activeValues[value] = true;
      }
   }

   ValueBits valueIsSolved;
   for (Field field : FieldBitsToRange(fields))
   {
      const auto &candidates = grid[field];
      const auto solved = candidates.tryGetSolvedValue();
      if (solved.has_value())
      {
         valueIsSolved.set(solved.value());
      }
   }

   ValueBits result;
   for (Value value : ValueAll)
   {
      if (activeValues[value] && !valueIsSolved.test(value))
      {
         result.set(value);
      }
   }
   return result;
}

FieldBits SudokuUtils::getFields(std::span<const Cell> cells)
{
   FieldBits result;
   for (const auto &cell : cells)
   {
      result.set(cell.field);
   }
   return result;
}

FieldBits SudokuUtils::getFields(std::span<const Field> fields)
{
   FieldBits result;
   for (auto f : fields)
   {
      result.set(f);
   }
   return result;
}

Field SudokuUtils::FirstField(const FieldBits &fb)
{
   for (Field field : FieldBitsToRange(fb))
   {
      return field;
   }
   throw MyException("FirstField empty");
}
