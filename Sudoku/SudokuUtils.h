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
} // namespace Sudoku
