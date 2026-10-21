#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "UniqueSorted.h"

using namespace Sudoku;
using namespace Utilities;

namespace str = std::ranges;

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

CandidatesInHouse::CandidatesInHouse(const CandidatesGrid &cg, House house) : m_grid(&cg), m_house(std::move(house))
{
   for (auto cell : m_house.cells)
   {
      const Field field = cell.field;
      const auto numCandidates = cg[field].size();
      m_multiplicities.at(numCandidates).push_back(field);
      for (auto value : ValueAll)
      {
         if (cg[field].contains(value))
         {
            m_fieldsContainingValue[value].push_back(field);
         }
      }
   }
}

const House &CandidatesInHouse::getHouse() const
{
   return m_house;
}

std::span<const Field> CandidatesInHouse::getFieldsContainingValue(Value value) const
{
   return m_fieldsContainingValue.at(value);
}

const CandidatesGrid &CandidatesInHouse::getGrid() const
{
   return *m_grid;
}

std::unordered_set<Value> SudokuUtils::getUniqueActiveValuesInCells(const CandidatesGrid &grid,
                                                                    std::span<const Field> fields)
{
   std::unordered_set<Value> result;
   for (Field f : fields)
   {
      const auto &candidates = grid[f];
      // Skip the known values
      if (candidates.isSolved())
      {
         continue;
      }
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

Index CandidatesInHouse::getNumCandidatesInCell(Field field) const
{
   return (*m_grid)[field].size();
}

bool CandidatesInHouse::isConsistentWith(const NakedTuplesInput &input) const
{
   return str::all_of(input.getMultiplicitiesToTry(), [this, &input](auto m) {
      return input.getMultiplicitySize(m) <= getFieldsWithMultiplicity(m).size();
   });
}
