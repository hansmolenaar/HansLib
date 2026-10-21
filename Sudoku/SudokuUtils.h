#pragma once

#include "House.h"
#include "SudokuDefines.h"

#include <map>
#include <unordered_set>
#include <vector>

namespace Sudoku
{

class CandidatesGrid;

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

class CandidatesInHouse
{
 public:
   CandidatesInHouse(const CandidatesGrid &cg, House house);
   bool isConsistentWith(const NakedTuplesInput &input) const;
   std::unordered_set<Value> getUniqueActiveValuesInCells(std::span<const Field> fields) const;
   std::span<const Field> getFieldsWithMultiplicity(Index multiplicity) const;
   std::span<const Field> getFieldsContainingValue(Value) const;
   Index getNumCandidatesInCell(Field) const;
   const House &getHouse() const;

 private:
   const CandidatesGrid *m_grid;
   House m_house;
   std::array<std::vector<Field>, NumValue + 1> m_multiplicities;
   std::array<std::vector<Field>, NumValue + 1> m_fieldsContainingValue;
};
} // namespace Sudoku
