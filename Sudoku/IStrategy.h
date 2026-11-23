#pragma once

#include "FieldValue.h"

namespace Sudoku
{
class FieldsInfo;

class IStrategy
{
 public:
   virtual ~IStrategy() = default;
   virtual FieldValue::Container collect(const FieldsInfo &) = 0;
};

} // namespace Sudoku
