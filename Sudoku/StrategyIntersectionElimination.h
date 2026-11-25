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
   std::string getName() const override;
   void collect(const FieldsInfo &, FieldValues &) override;

   static std::optional<House> TryGetIntersectionHouse(HouseType, const FieldBits &, const FieldsInfo &);
   static FieldValueCount collectHouse(const FieldsInfo &, const House &, FieldValues &);
};

} // namespace Sudoku
