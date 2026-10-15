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
#include <set>
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

class NakedTuplesInput
{
 public:
   NakedTuplesInput(std::initializer_list<Index>);
   Index getNumUniqueValues() const;
   std::span<const Index> getMultiplicitiesToTry() const;
   Index getMultiplicitySize(Index) const;

 private:
   Index m_numUniqueValues;
   std::vector<Index> m_multiplicites;
   std::map<Index, Index> m_multiplicitesCounted;
};

NakedTuplesInput::NakedTuplesInput(std::initializer_list<Index> useCandidatesOfSize)
    : m_numUniqueValues(useCandidatesOfSize.size()), m_multiplicites(useCandidatesOfSize)
{
   for (auto m : m_multiplicites)
   {
      m_multiplicitesCounted[m] += 1;
   }
   UniqueSorted(m_multiplicites);
}

Index NakedTuplesInput::getNumUniqueValues() const
{
   return m_numUniqueValues;
}

std::span<const Index> NakedTuplesInput::getMultiplicitiesToTry() const
{
   return m_multiplicites;
}

Index NakedTuplesInput::getMultiplicitySize(Index multiplicity) const

{
   return m_multiplicitesCounted.at(multiplicity);
}

class CandidatesInHouse
{
 public:
   CandidatesInHouse(const CandidatesGrid &cg, House house);
   bool isConsistentWith(const NakedTuplesInput &input) const;
   std::unordered_set<Value> getUniqueValuesInCells(std::span<const Field> fields) const;
   std::span<const Field> getFieldsWithMultiplicity(Index multiplicity) const;
   const House &getHouse() const;

 private:
   const CandidatesGrid &m_grid;
   House m_house;
   // std::array<Index, NumCellsInHouse> m_candidateMultiplicity;
   std::array<std::vector<Field>, NumValue + 1> m_multiplicities;
};

CandidatesInHouse::CandidatesInHouse(const CandidatesGrid &cg, House house) : m_grid(cg), m_house(std::move(house))
{
   for (auto cell : m_house.cells)
   {
      const Field field = cell.field;
      const auto numCandidates = cg[field].size();
      m_multiplicities.at(numCandidates).push_back(field);
   }
}

const House &CandidatesInHouse::getHouse() const
{
   return m_house;
}

std::unordered_set<Value> CandidatesInHouse::getUniqueValuesInCells(std::span<const Field> fields) const
{
   std::unordered_set<Value> result;
   for (Field f : fields)
   {
      const auto &candidates = m_grid[f];
      for (Value value : ValueAll)
      {
         if (candidates.contains(value))
         {
            result.insert(value);
         }
      }
   }
   return result;
}

std::span<const Field> CandidatesInHouse::getFieldsWithMultiplicity(Index multiplicity) const
{
   return m_multiplicities.at(multiplicity);
}

bool CandidatesInHouse::isConsistentWith(const NakedTuplesInput &input) const
{
   return str::all_of(input.getMultiplicitiesToTry(), [this, &input](auto m) {
      return input.getMultiplicitySize(m) <= getFieldsWithMultiplicity(m).size();
   });
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
