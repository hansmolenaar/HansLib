#pragma once

#include "House.h"
#include "SudokuDefines.h"

#include <bitset>
#include <boost/container/static_vector.hpp>

namespace Sudoku
{

class CandidatesGrid;

namespace SudokuUtils
{
boost::container::static_vector<Value, NumValue> getUniqueActiveValuesInCells(const CandidatesGrid &,
                                                                              std::span<const Cell>);
Index countOccurence(const CandidatesGrid &, std::span<const Cell> cells, Value value);
boost::container::static_vector<Cell, NumCellsInHouse> getNotSolvedCells(const CandidatesGrid &, const House &);
std::bitset<NumValue + 1> getSolvedValues(const CandidatesGrid &, std::span<const Cell>);
std::bitset<NumValue + 1> getSolvedValues(const CandidatesGrid &, std::span<const Field>);

using Value2Fields = std::array<boost::container::static_vector<Field, NumField>, NumValue + 1>;
Value2Fields getValue2Fields(const CandidatesGrid &, std::span<Field>);
} // namespace SudokuUtils
} // namespace Sudoku
