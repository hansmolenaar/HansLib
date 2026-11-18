#pragma once

#include "BitsetUtils.h"
#include "Cell.h"
#include "SudokuDefines.h"

#include <boost/container/static_vector.hpp>

namespace Sudoku
{

class CandidatesGrid;

namespace SudokuUtils
{
ValueBits getUniqueActiveValuesInCells(const CandidatesGrid &, const FieldBits &);
Index countOccurence(const CandidatesGrid &, std::span<const Cell> cells, Value value);

using Value2Fields = std::array<boost::container::static_vector<Field, NumField>, NumValue + 1>;
Value2Fields getValue2Fields(const CandidatesGrid &, std::span<Field>);
Value2Fields getValue2Fields(const CandidatesGrid &, std::span<const Cell>);

FieldBits getFields(std::span<const Cell>);
FieldBits getFields(std::span<const Field>);

inline auto FieldBitsToRange(const FieldBits &fb)
{
   return FieldAll | std::views::filter(Utilities::Bitset{fb});
}

} // namespace SudokuUtils
} // namespace Sudoku
