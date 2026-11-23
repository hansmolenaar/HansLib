#pragma once

#include "House.h"
#include "IStrategy.h"
#include <optional>

namespace Sudoku
{
// In TryGetIntersectionHouse
class StrategyIntersectionElimination : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;

   static std::optional<House> TryGetIntersectionHouse(HouseType, const FieldBits &, const FieldsInfo &);
   static FieldValue::Container collectHouse(const FieldsInfo &, const House &);
};

} // namespace Sudoku
