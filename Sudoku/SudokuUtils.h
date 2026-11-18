#pragma once

#include "BitsetUtils.h"
#include "Cell.h"
#include "MyAssert.h"
#include "SudokuDefines.h"

#include <algorithm>
#include <boost/container/static_vector.hpp>
#include <iterator>
#include <optional>
#include <ranges>

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

Field FirstField(const FieldBits &);

inline auto FieldBitsToRange(const FieldBits &fb)
{
   return FieldAll | std::views::filter(Utilities::Bitset{fb});
}

// Convenience function for testing
inline std::vector<Field> FieldBitsToVector(const FieldBits &fb)
{
   std::vector<Field> result;
   std::ranges::copy(FieldBitsToRange(fb), std::back_inserter(result));
   return result;
}

inline Field SingleField(const FieldBits &fb)
{
   std::optional<Field> result;
   for (Field field : FieldBitsToRange(fb))
   {
      Utilities::MyAssert(!result.has_value(), "Single: more than 1 field bit set");
      result = field;
   }
   Utilities::MyAssert(result.has_value(), "Single: no field bits set");
   return result.value();
}

} // namespace SudokuUtils
} // namespace Sudoku
