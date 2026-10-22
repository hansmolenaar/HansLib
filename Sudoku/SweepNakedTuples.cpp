#include "SweepNakedTuples.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "Contains.h"
#include "House.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"

#include <algorithm>
#include <initializer_list>
#include <numeric>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{

bool RemoveCandidates(CandidatesGrid &cg, const CandidatesInHouse &candidatesInHouse,
                      std::span<const Field> candidateTuple)
{
   const auto numCandidates = cg.getNumCandidates();
   const auto candidatesToRemove = SudokuUtils::getUniqueActiveValuesInCells(cg, candidateTuple);
   for (const Cell &cell : candidatesInHouse.getHouse().cells)
   {
      if (!Contains(candidateTuple, cell.field))
      {
         for (auto v : candidatesToRemove)
         {
            cg.removeCandidate(cell.field, v);
         }
      }
   }

   return cg.getNumCandidates() != numCandidates;
}

std::vector<Value> GetUniqueValuesFromFields(const CandidatesGrid &grid, std::span<const Field> fields)
{
   std::vector<Value> result;
   for (Field field : fields)
   {
      for (Value value : ValueAll)
      {
         if (grid[field].contains(value))
         {
            result.push_back(value);
         }
      }
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

bool IsNakedTuple(const CandidatesInHouse &candidatesInHouse, std::span<const Field> candidateTuple,
                  NakedTuplesInput input)
{
   const auto values = SudokuUtils::getUniqueActiveValuesInCells(candidatesInHouse.getGrid(), candidateTuple);
   if (values.size() != candidateTuple.size())
   {
      return false;
   }
   std::array<Index, NumCellsInHouse> multiplitcityCount;
   std::fill_n(multiplitcityCount.begin(), NumCellsInHouse, 0); // TODO is this necessary?
   for (auto c : candidateTuple)
   {
      multiplitcityCount[candidatesInHouse.getNumCandidatesInCell(c)] += 1;
   }
   return str::all_of(input.getMultiplicitiesToTry(), [&input, &multiplitcityCount](auto m) {
      return input.getMultiplicitySize(m) == multiplitcityCount[m];
   });
}
bool IsNakedTuplE(const CandidatesGrid &grid, const std::vector<Field> &candidateTuple) // TODO
{
   const auto values = GetUniqueValuesFromFields(grid, candidateTuple);
   return values.size() == candidateTuple.size();
}

} // namespace

bool SweepNakedTuples::sweepHouseOfType(CandidatesGrid &cg, HouseType houseType,
                                        std::initializer_list<Index> useCandidatesOfSize)
{
   bool anyChange = false;
   const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);

   for (const Index &houseIndex : IndexAll)
   {
      CandidatesInHouse candidatesInHouse(cg, House::get(houseType, houseIndex));
      if (candidatesInHouse.isConsistentWith(nakedTupleInput))
      {
         std::vector<Field> fieldsToTest;
         for (auto multiplicity : useCandidatesOfSize)
         {
            const auto fields = candidatesInHouse.getFieldsWithMultiplicity(multiplicity);
            fieldsToTest.insert(fieldsToTest.end(), fields.begin(), fields.end());
         }
         UniqueSorted(fieldsToTest);
         Combinations<Field> combinations(fieldsToTest, nakedTupleInput.getNumUniqueValues());
         do
         {
            if (IsNakedTuple(candidatesInHouse, combinations(), nakedTupleInput))
            {
               const bool anyChangeInHouse = RemoveCandidates(cg, candidatesInHouse, combinations());
               if (anyChangeInHouse)
               {
                  anyChange = true;
                  // candidatesInHouse is stale now, recalculate it
                  candidatesInHouse = CandidatesInHouse(cg, House::get(houseType, houseIndex));
               }
            }
         } while (combinations.next());
      }
   }
   return anyChange;
}

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
      if (IsNakedTuplE(grid, combinations()))
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

bool SweepNakedTuples::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType houseType : HouseTypeAll)
   {
      anyChange |= sweepHouseOfType(cg, houseType, {2, 2});
   }
   return anyChange;
}
