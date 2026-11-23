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

class SweepXWing : public IStrategy
{
 public:
   FieldValue::Container collect(const FieldsInfo &) override;
   static std::optional<XWing> isXWing(std::span<const LockedPair>);
};

} // namespace Sudoku
