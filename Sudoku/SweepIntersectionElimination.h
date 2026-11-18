#pragma once

#include "House.h"
#include "ISweep.h"
#include <optional>

namespace Sudoku
{
// In TryGetIntersectionHouse
class SweepIntersectionElimination : public ISweep
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;

   static std::optional<HouseId> TryGetIntersectionHouse(HouseType, std::span<const Cell>);
   static FieldValue::Container collectHouse(const FieldsInfo &, const HouseId &);
};

} // namespace Sudoku
