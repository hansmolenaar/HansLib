#pragma once

#include "House.h"
#include "IStrategy.h"
#include <optional>

namespace Sudoku
{
// In TryGetIntersectionHouse
class SweepIntersectionElimination : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;

   static std::optional<House> TryGetIntersectionHouse(HouseType, const FieldBits &, const FieldsInfo &);
   static FieldValue::Container collectHouse(const FieldsInfo &, const House &);
};

} // namespace Sudoku
