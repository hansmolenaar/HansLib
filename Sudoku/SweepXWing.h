#pragma once

#include "House.h"
#include "ISweep.h"
#include "LockedPairs.h"
#include <optional>
#include <span>

namespace Sudoku
{
class LockedPair;

struct XWing
{
   LockedPair lockedPair0;
   HouseId houseId0;
   LockedPair lockedPair1;
   HouseId houseId1;
};

class SweepXWing : public ISweep
{
 public:
   bool sweep(CandidatesGrid &) override;
   static std::optional<XWing> isXWing(std::span<const LockedPair>);
};

} // namespace Sudoku
