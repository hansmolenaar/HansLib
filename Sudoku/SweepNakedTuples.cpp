#include "SweepNakedTuples.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "House.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"

#include <algorithm>
#include <initializer_list>
#include <unordered_set>

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
      if (str::find(candidateTuple, cell.field) == candidateTuple.end())
      {
         for (auto v : candidatesToRemove)
         {
            cg.removeCandidate(cell.field, v);
         }
      }
   }

   return cg.getNumCandidates() != numCandidates;
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

} // namespace

bool SweepNakedTuples::sweepHouseOfType(CandidatesGrid &cg, HouseType houseType,
                                        std::initializer_list<Index> useCandidatesOfSize)
{
   bool anyChange = false;
   const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);
   std::vector<Field> candidateTuple(nakedTupleInput.getNumUniqueValues());

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
         auto generator = make_combination_generator(fieldsToTest.begin(), fieldsToTest.end(), candidateTuple.begin(),
                                                     candidateTuple.end());
         do
         {
            if (IsNakedTuple(candidatesInHouse, candidateTuple, nakedTupleInput))
            {
               const bool anyChangeInHouse = RemoveCandidates(cg, candidatesInHouse, candidateTuple);
               if (anyChangeInHouse)
               {
                  anyChange = true;
                  // candidatesInHouse is stale now, recalculate it
                  candidatesInHouse = CandidatesInHouse(cg, House::get(houseType, houseIndex));
               }
            }
         } while (generator());
      }
   }
   return anyChange;
}

#if false
bool SweepNakedTuples::sweepHouseOfType(CandidatesGrid &grid, HouseType houseType, Index houseIndex, Index tupleSize)

{
   bool anyChange = false;
   const House &house = House::get(houseType, houseIndex);

   std::vector<Value> tupleTry(tupleSize);
   auto generator =
       make_combination_generator(activeValues.begin(), activeValues.end(), tupleTry.begin(), tupleTry.end());
   do
   {
      if (IsNakedTuplee(grid, house, tupleTry))
      {
         anyChange |= RemoveCandidates(grid, house, candidateTuple);
      }
   } while (generator());

   return anyChange;
}
#endif

bool SweepNakedTuples::sweep(CandidatesGrid &cg)
{
   bool anyChange = false;
   for (HouseType houseType : HouseTypeAll)
   {
      anyChange |= sweepHouseOfType(cg, houseType, {2, 2});
   }
   return anyChange;
}
