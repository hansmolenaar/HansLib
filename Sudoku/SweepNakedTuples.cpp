#include "SweepNakedTuples.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "House.h"
#include "Single.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include <algorithm>
#include <initializer_list>

#include <map>
#include <unordered_set>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
bool RemoveCandidates(CandidatesGrid &cg, const House &house, const std::unordered_set<Value> &candidatesToRemove,
                      const std::vector<Field> &fieldToSkip)
{
   const auto numCandidates = cg.getNumCandidates();
   for (const Cell &cell : house.cells)
   {
      if (str::find(fieldToSkip, cell.field) == fieldToSkip.end())
      {

         for (auto v : candidatesToRemove)
         {
            cg.removeCandidate(cell.field, v);
         }
      }
   }
   return cg.getNumCandidates() != numCandidates;
}

} // namespace

void SweepNakedTuples::sweepHouseOfType(CandidatesGrid &cg, HouseType houseType,
                                        std::initializer_list<Index> useCandidatesOfSize)
{
   const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);
   const auto numUniqueValues = nakedTupleInput.getNumUniqueValues();

   const Index onlyUseMultiplicity = Single(nakedTupleInput.getMultiplicitiesToTry());

   std::vector<Field> fieldCombination(onlyUseMultiplicity);
   for (const Index &houseIndex : IndexAll)
   {
      const CandidatesInHouse candidatesInHouse(cg, House::get(houseType, houseIndex));
      if (candidatesInHouse.isConsistentWith(nakedTupleInput))
      {
         const auto &fieldsToTest = candidatesInHouse.getFieldsWithMultiplicity(onlyUseMultiplicity);
         auto generator = make_combination_generator(fieldsToTest.begin(), fieldsToTest.end(), fieldCombination.begin(),
                                                     fieldCombination.end());
         do
         {
            const std::unordered_set<Value> values = candidatesInHouse.getUniqueValuesInCells(fieldCombination);
            if (values.size() == numUniqueValues)
            {
               // Naked tuple found
               const bool anyChange = RemoveCandidates(cg, candidatesInHouse.getHouse(), values, fieldCombination);
               if (anyChange)
               {
                  return; // candidatesInHouse is be stale now
               }
            }

         } while (generator());
      }
   }
}

void SweepNakedTuples::sweep(CandidatesGrid &cg)
{
   for (HouseType houseType : HouseTypeAll)
   {
      sweepHouseOfType(cg, houseType, {2, 2});
   }
}
