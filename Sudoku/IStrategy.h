#pragma once

#include "FieldValue.h"

namespace Sudoku
{
class FieldsInfo;

class IStrategy
{
 public:
   virtual ~IStrategy() = default;
   virtual void collect(const FieldsInfo &, FieldValues &) = 0;
   FieldValues collect(const FieldsInfo &);
};

} // namespace Sudoku
