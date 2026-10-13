#include "SweepNakedTuples.h"
#include "CandidatesGrid.h"
#include "Combinations.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include <algorithm>
#include <initializer_list>

#include <map>
#include <set>

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

namespace
{
bool RemoveCandidates(CandidatesGrid &cg, const House &house, const std::set<Value> &candidatesToRemove,
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
   const auto numUniqueValues = useCandidatesOfSize.size();
   std::map<Index, Index> multiplicityCount;
   for (Index m : useCandidatesOfSize)
   {
      multiplicityCount[m] += 1;
   }
   MyAssert(multiplicityCount.size() == 1); // TODO
   const Index onlyUseMultiplicity = multiplicityCount.begin()->first;

   std::vector<Field> fieldCombination(onlyUseMultiplicity);
   std::set<Value> values;
   for (const Index &houseIndex : IndexAll)
   {
      const auto &house = House::get(houseType, houseIndex);
      const HouseByMultiplicities multiplicitiesInHouse(cg, house);
      if (str::all_of(multiplicityCount, [&multiplicitiesInHouse](auto itr) {
             return itr.second <= multiplicitiesInHouse.get(itr.first).size();
          }))
      {
         const auto &fieldsToTest = multiplicitiesInHouse.get(onlyUseMultiplicity);
         auto generator = make_combination_generator(fieldsToTest.begin(), fieldsToTest.end(), fieldCombination.begin(),
                                                     fieldCombination.end());
         do
         {
            values.clear();
            for (auto field : fieldCombination)
            {
               for (Value v : ValueAll)
               {
                  if (cg[field].contains(v))
                  {
                     values.insert(v);
                  }
               }
            }
            if (values.size() == numUniqueValues)
            {
               // Naked tuple found
               const bool anyChange = RemoveCandidates(cg, house, values, fieldCombination);
               if (anyChange)
               {
                  return; // multiplicitiesInHouse could be stale now
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
