#include "Coordinates.h"
#include "MyAssert.h"

using namespace Sudoku;
using namespace Utilities;

namespace
{
void CheckValid(const Coordinates c)
{
   MyAssert(c.Row < NumIndex && c.Col < NumIndex);
}
} // namespace

Field Coordinates::toField() const
{
   CheckValid(*this);
   return Row * NumIndex + Col;
}

Coordinates Coordinates::fromField(Field f)
{
   Coordinates result;
   result.Row = f / NumIndex;
   result.Col = f - result.Row * NumIndex;
   CheckValid(result);
   return result;
}
