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
   const auto candidatesToRemove = candidatesInHouse.getUniqueValuesInCells(candidateTuple);
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

bool IsNakedTuple(const CandidatesInHouse &candidatesInHouse, std::span<const Field> candidateTuple)
{
   const auto values = candidatesInHouse.getUniqueValuesInCells(candidateTuple);
   return values.size() == candidateTuple.size();
}

} // namespace

void SweepNakedTuples::sweepHouseOfType(CandidatesGrid &cg, HouseType houseType,
                                        std::initializer_list<Index> useCandidatesOfSize)
{
   const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);
   std::vector<Field> candidateTuple(nakedTupleInput.getNumUniqueValues());

   for (const Index &houseIndex : IndexAll)
   {
      const CandidatesInHouse candidatesInHouse(cg, House::get(houseType, houseIndex));
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
            if (IsNakedTuple(candidatesInHouse, candidateTuple))
            {
               const bool anyChange = RemoveCandidates(cg, candidatesInHouse, candidateTuple);
               if (anyChange)
               {
                  return; // candidatesInHouse is stale now
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
