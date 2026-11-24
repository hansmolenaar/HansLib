#include "FieldValue.h"
#include "BitsetUtils.h"

using namespace Sudoku;
using namespace Utilities;

// *********************** FieldValues

FieldValueCount FieldValues::add(FieldValue fv)
{
   if (!m_fieldValuesSorted.contains(fv))
   {
      m_fieldValuesSorted.insert(fv);
      m_fieldValues.push_back(fv);
      return 1;
   }
   return 0;
}

FieldValueCount FieldValues::add(Field field, Value value)
{
   return add({field, value});
}

FieldValueCount FieldValues::add(const FieldBits &fields, Value value)
{
   FieldValueCount result = 0;
   for (Field field : FieldAll | std::views::filter(Bitset{fields}))
   {
      result += add(field, value);
   }
   return result;
}

FieldValueCount FieldValues::add(const FieldValues &fvs)
{
   FieldValueCount result = 0;
   for (auto fv : fvs.get())
   {
      result += add(fv);
   }
   return result;
}

std::span<const FieldValue> FieldValues::get() const
{
   return m_fieldValues;
}

size_t FieldValues::size() const
{
   return m_fieldValues.size();
}
