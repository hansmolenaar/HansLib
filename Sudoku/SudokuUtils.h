#pragma once

#include "Cell.h"
#include "SudokuDefines.h"

#include <bitset>
#include <boost/container/static_vector.hpp>

namespace Sudoku
{

class CandidatesGrid;

namespace SudokuUtils
{
ValueBits getUniqueActiveValuesInCells(const CandidatesGrid &, std::span<const Cell>);
Index countOccurence(const CandidatesGrid &, std::span<const Cell> cells, Value value);
ValueBits getSolvedValues(const CandidatesGrid &, std::span<const Cell>);
ValueBits getSolvedValues(const CandidatesGrid &, std::span<const Field>);

using Value2Fields = std::array<boost::container::static_vector<Field, NumField>, NumValue + 1>;
Value2Fields getValue2Fields(const CandidatesGrid &, std::span<Field>);
Value2Fields getValue2Fields(const CandidatesGrid &, std::span<const Cell>);

FieldBits getNotSolvedCells(const CandidatesGrid &, std::span<const Cell>);
FieldBits getNotSolvedCells(const CandidatesGrid &);
FieldBits getSolvedCells(const CandidatesGrid &);
FieldBits getFields(std::span<const Cell>);

} // namespace SudokuUtils
} // namespace Sudoku
