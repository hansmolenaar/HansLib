#pragma once

#include "FieldValue.h"

namespace Sudoku
{
class FieldsInfo;

class IStrategy
{
 public:
   virtual ~IStrategy() = default;
   virtual bool collect(const FieldsInfo &, FieldValue::Container &) = 0;
   FieldValue::Container collect(const FieldsInfo &);
};

} // namespace Sudoku
