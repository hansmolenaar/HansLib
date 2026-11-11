#pragma once

#include <bitset>
#include <ranges>

namespace Sudoku
{
using Value = unsigned short; // 1..9
constexpr Value NumValue{9};
constexpr Value ValueUndefined{10};
constexpr auto ValueAll = std::ranges::iota_view{static_cast<Value>(1), ValueUndefined};

using Index = unsigned short; // 0..8
constexpr Index NumIndex(9);
constexpr Index IndexUndefined = NumIndex;
constexpr auto IndexAll = std::ranges::iota_view{static_cast<Index>(0), NumIndex};

using Field = unsigned short; // 0..80
constexpr Field NumField = NumIndex * NumIndex;
constexpr Field FieldUndefined = NumField;
constexpr auto FieldAll = std::ranges::iota_view{static_cast<Field>(0), NumField};
using FieldBits = std::bitset<NumField>;

using BoxIndex = unsigned short; // 0..2
constexpr BoxIndex NumBoxIndex(3);
constexpr auto BoxIndexAll = std::ranges::iota_view{static_cast<BoxIndex>(0), NumBoxIndex};

constexpr auto NumCellsInHouse = NumIndex;
enum class HouseType
{
   Row,
   Col,
   Box
};
constexpr std::array<HouseType, 3> HouseTypeAll = {HouseType::Row, HouseType::Col, HouseType::Box};
constexpr Index NumHouse = 3 * NumIndex;

constexpr char ValueUndefinedChar = '.';

enum class LinkType
{
   Strong,
   Weak
};
} // namespace Sudoku
