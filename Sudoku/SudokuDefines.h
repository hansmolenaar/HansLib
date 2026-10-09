#pragma once

#include <array>

namespace Sudoku
{
using Value = unsigned short; // 1..9
constexpr Value NumValue{9};
constexpr Value ValueUndefined{0};
constexpr std::array<Value, NumValue> ValueAll = {1, 2, 3, 4, 5, 6, 7, 8, 9};

using Index = unsigned short; // 0..8
constexpr Index NumIndex(9);
constexpr Index IndexUndefined = NumIndex;
constexpr std::array<Index, NumIndex> IndexAll = {0, 1, 2, 3, 4, 5, 6, 7, 8};

using Field = unsigned short; // 0..80
constexpr Field NumField = NumIndex * NumIndex;
constexpr Field FieldUndefined = NumField;

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
