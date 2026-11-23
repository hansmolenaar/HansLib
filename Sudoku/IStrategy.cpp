#include "IStrategy.h"

using namespace Sudoku;

FieldValue::Container IStrategy::collect(const FieldsInfo &finfo)
{
   FieldValue::Container result;
   collect(finfo, result);
   return result;
}
