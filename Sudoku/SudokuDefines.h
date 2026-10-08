#pragma once

#include <array>

namespace Sudoku
{
using Value = unsigned short; // 1..9
constexpr Value ValueUndefined{0};
constexpr Value NumValues{9};
constexpr std::array<Value, NumValues> ValueAll = {Value(1), Value(2), Value(3), Value(4), Value(5),
                                                   Value(6), Value(7), Value(8), Value{9}};
constexpr Value ValueMax = ValueAll.back();

using RowColBox = unsigned short; // 0..8

constexpr RowColBox NumRowColBox(9);
constexpr RowColBox NumRow = NumRowColBox;
constexpr RowColBox NumCol = NumRowColBox;
constexpr RowColBox NumBox = NumRowColBox;

constexpr std::array<RowColBox, NumRowColBox> RowColBoxAll = {RowColBox(0), RowColBox(1), RowColBox(2),
                                                              RowColBox(3), RowColBox(4), RowColBox(5),
                                                              RowColBox(6), RowColBox(7), RowColBox(8)};
constexpr std::array<RowColBox, NumRow> RowAll = RowColBoxAll;
constexpr std::array<RowColBox, NumCol> ColAll = RowColBoxAll;
constexpr std::array<RowColBox, NumBox> BoxAll = RowColBoxAll;

using Field = unsigned short; // 0..80
constexpr Field NumFields = NumRow * NumCol;

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
