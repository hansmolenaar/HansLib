#include "SweepNakedTuples.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "House.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include <algorithm>
#include <iterator>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

std::vector<Value> GetUniqueValuesFromFields(const CandidatesGrid &grid, std::span<const Field> fields)
{
   std::vector<Value> result;
   for (Field field : fields)
   {
      str::copy(grid[field], std::back_inserter(result));
   }
   UniqueSorted(result);
   return result;
}

bool RemoveCandidates(CandidatesGrid &grid, const House &house, std::span<const Field> candidateTuple)
{
   bool anyChange = false;
   const auto candidatesToRemove = GetUniqueValuesFromFields(grid, candidateTuple);
   for (const Cell &cell : house.cells)
   {
      if (!Contains(candidateTuple, cell.field))
      {
         for (auto v : candidatesToRemove)
         {
            anyChange |= grid.removeCandidate(cell.field, v);
         }
      }
   }

   return anyChange;
}

bool IsNakedTuple(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple)
{
   const auto values = GetUniqueValuesFromFields(grid, candidateTuple);
   return values.size() == candidateTuple.size();
}

} // namespace

bool SweepNakedTuples::sweepHouse(CandidatesGrid &grid, const House &house, Index tupleSize)

{
   bool anyChange = false;

   std::vector<Field> activeFields;
   for (Cell cell : house.cells)
   {
      if (!grid[cell.field].isSolved() && grid[cell.field].size() <= tupleSize)
      {
         activeFields.push_back(cell.field);
      }
   }

   if (activeFields.size() < tupleSize)
   {
      return anyChange;
   }

   Combinations<Field> combinations(activeFields, tupleSize);
   do
   {
      if (IsNakedTuple(grid, combinations()))
      {
         anyChange |= RemoveCandidates(grid, house, combinations());
      }
   } while (combinations.next());

   return anyChange;
}

bool SweepNakedTuples::sweepHousesOfType(CandidatesGrid &grid, HouseType type, Index tupleSize)
{
   bool anyChange = false;
   for (Index idx : IndexAll)
   {
      anyChange |= sweepHouse(grid, House::get(type, idx), tupleSize);
   }
   return anyChange;
}

bool SweepNakedTuples::sweep(CandidatesGrid &cg, Index tupleSize)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= sweepHousesOfType(cg, type, tupleSize);
   };
   return anyChange;
}

// ***************************************************************

bool SweepNakedPairs::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(cg, type, 2);
   };
   return anyChange;
}

// ***************************************************************

bool SweepNakedTriplet::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(cg, type, 3);
   };
   return anyChange;
}

// ***************************************************************

bool SweepNakedQuartet::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType type : HouseTypeAll)
   {
      anyChange |= SweepNakedTuples::sweepHousesOfType(cg, type, 4);
   };
   return anyChange;
}
