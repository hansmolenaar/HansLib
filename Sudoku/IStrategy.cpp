#include "IStrategy.h"

using namespace Sudoku;

FieldValue::Container IStrategy::collect(const FieldsInfo &)
{
   throw std::runtime_error("IStrategy::collect");
}
