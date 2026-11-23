#pragma once

#include "House.h"
#include "IStrategy.h"
#include "LockedPairs.h"
#include <optional>
#include <span>

namespace Sudoku
{
class LockedPair;

struct XWing
{
   LockedPair lockedPair0;
   House house0;
   LockedPair lockedPair1;
   House house1;
};

class StrategyXWing : public IStrategy
{
 public:
   bool collect(const FieldsInfo &, FieldValue::Container &) override;

   static std::optional<XWing> isXWing(std::span<const LockedPair>);
};

} // namespace Sudoku
