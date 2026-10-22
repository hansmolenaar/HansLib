#pragma once

#include "House.h"
#include "SudokuDefines.h"

#include <boost/container/static_vector.hpp>
#include <map>
#include <set>
#include <vector>

namespace Sudoku
{

class CandidatesGrid;

namespace SudokuUtils
{
std::set<Value> getUniqueActiveValuesInCells(const CandidatesGrid &, std::span<const Cell>);
std::set<Value> getUniqueActiveValuesInCells(const CandidatesGrid &, std::span<const Field>);
Index countOccurence(const CandidatesGrid &, std::span<const Cell> cells, Value value);
boost::container::static_vector<Cell, NumCellsInHouse> getNotSolvedCells(const CandidatesGrid &, const House &);

} // namespace SudokuUtils

class CandidatesInHouse
{
   CandidatesInHouse(const CandidatesGrid &cg, House house);

 public:
   std::span<const Field> getFieldsWithMultiplicity(Index multiplicity) const;
   std::span<const Field> getFieldsContainingValue(Value) const;
   Index getNumCandidatesInCell(Field) const;
   const House &getHouse() const;
   const CandidatesGrid &getGrid() const;

 private:
   const CandidatesGrid *m_grid;
   House m_house;
   std::array<std::vector<Field>, NumValue + 1> m_multiplicities;
   std::array<std::vector<Field>, NumValue + 1> m_fieldsContainingValue;
};
} // namespace Sudoku
