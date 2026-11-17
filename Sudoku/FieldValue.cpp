#include "FieldValue.h"
#include "BitsetUtils.h"

using namespace Sudoku;
using namespace Utilities;

void FieldValue::add(const FieldBits &fields, Value value, Container &fieldValues)
{
   for (Field field : FieldAll | std::views::filter(Bitset{fields}))
   {
      fieldValues.insert({field, value});
   }
}
