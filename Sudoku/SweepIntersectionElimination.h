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
   bool sweep(CandidatesGrid &) override;
   static std::optional<HouseId> TryGetIntersectionHouse(HouseType, std::span<const Field>);
};

} // namespace Sudoku
