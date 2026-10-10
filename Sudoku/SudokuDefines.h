#pragma once

#include <ranges>

namespace Sudoku
{
using Value = unsigned short; // 1..9
constexpr Value NumValue{9};
constexpr Value ValueUndefined{0};
constexpr auto ValueAll = std::ranges::iota_view{static_cast<Value>(1), NumValue};

using Index = unsigned short; // 0..8
constexpr Index NumIndex(9);
constexpr Index IndexUndefined = NumIndex;
constexpr auto IndexAll = std::ranges::iota_view{static_cast<Index>(0), NumIndex};

using Field = unsigned short; // 0..80
constexpr Field NumField = NumIndex * NumIndex;
constexpr Field FieldUndefined = NumField;
constexpr auto FieldAll = std::ranges::iota_view{static_cast<Field>(0), NumField};

constexpr char ValueUndefinedChar = '.';

#if false
using RowColBoxPosition = unsigned short;                         // Position in row/col/box
constexpr RowColBoxPosition NumRowColBoxPositions = 9; // Number of RowColBoxPosition in row/col/box
constexpr std::array<RowColBoxPosition, NumRowColBoxPositions> RowColBoxPositionsAll = {0, 1, 2, 3, 4, 5, 6, 7, 8};


using FieldSet = std::array<FieldIndex, NumRowColBoxPositions>;
using ValueSet = std::array<Value, NumRowColBoxPositions>;

constexpr FieldIndex NumConnectedFields = 2 * (NumRowColBox - 1) + 4;
using ConnectedFields = std::array<FieldIndex, NumConnectedFields>;
using ActiveFields = boost::container::static_vector<FieldIndex, NumFields>;

struct FieldValue
{
   FieldIndex Field;
   Value CurrentValue;
};
#endif
} // namespace Sudoku
