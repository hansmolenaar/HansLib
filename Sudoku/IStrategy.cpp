#include "IStrategy.h"

using namespace Sudoku;

FieldValues IStrategy::collect(const FieldsInfo &finfo)
{
   FieldValues result;
   collect(finfo, result);
   return result;
}
