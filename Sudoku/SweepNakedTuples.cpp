#include "SweepNakedTuples.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "House.h"
#include "Single.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"

#include <algorithm>
#include <array>
#include <initializer_list>
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

bool RemoveCandidates2(CandidatesGrid &cg, const CandidatesInHouse &candidatesInHouse,
                       std::array<Field, 2> candidateTuple)
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

bool IsNakedTuple(const CandidatesInHouse &candidatesInHouse, std::array<Field, 2> candidateTuple,
                  const NakedTuplesInput &input)
{
   const auto values = candidatesInHouse.getUniqueValuesInCells(candidateTuple);
   return (values.size() == input.getNumUniqueValues());
}

} // namespace

void SweepNakedTuples::sweepHouseOfType(CandidatesGrid &cg, HouseType houseType,
                                        std::initializer_list<Index> useCandidatesOfSize)
{
   const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);

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
         if (nakedTupleInput.getNumUniqueValues() == 2)
         {
            for (Index pos0 = 0; pos0 < fieldsToTest.size(); ++pos0)
            {
               for (Index pos1 = pos0 + 1; pos1 < fieldsToTest.size(); ++pos1)
               {
                  std::array<Field, 2> candidateTuple = {fieldsToTest[0], fieldsToTest[1]};
                  if (IsNakedTuple(candidatesInHouse, candidateTuple, nakedTupleInput))
                  {
                     const bool anyChange = RemoveCandidates2(cg, candidatesInHouse, candidateTuple);
                     if (anyChange)
                     {
                        return; // candidatesInHouse is be stale now
                     }
                  }
               }
            }
         }
      }
   }
}

void SweepNakedTuples::sweepHouseOfType2(CandidatesGrid &cg, HouseType houseType,
                                         std::initializer_list<Index> useCandidatesOfSize)
{
   const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);
   const auto numUniqueValues = nakedTupleInput.getNumUniqueValues();
   const Index onlyUseMultiplicity = Single(nakedTupleInput.getMultiplicitiesToTry());
   // const NakedTuplesInput nakedTupleInput(useCandidatesOfSize);
   // const Index onlyUseMultiplicity = Single(nakedTupleInput.getMultiplicitiesToTry()); // TODO
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
      sweepHouseOfType2(cg, houseType, {2, 2});
   }
}
