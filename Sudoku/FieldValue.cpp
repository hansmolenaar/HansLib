#include "FieldValue.h"
#include "BitsetUtils.h"

using namespace Sudoku;
using namespace Utilities;

bool FieldValue::add(const FieldBits &fields, Value value, Container &fieldValues)
{
   const auto sizeBefore = fieldValues.size();
   for (Field field : FieldAll | std::views::filter(Bitset{fields}))
   {
      fieldValues.insert({field, value});
   }
   return fieldValues.size() != sizeBefore;
}

void FieldValue::add(const Container &toAdd, Container &sum)
{
   sum.insert(toAdd.begin(), toAdd.end());
}
