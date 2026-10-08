#include "Coordinates.h"
#include "MyAssert.h"

using namespace Sudoku;
using namespace Utilities;

namespace
{
void CheckValid(const Coordinates c)
{
   MyAssert(c.Row < NumRow && c.Col < NumCol);
}
} // namespace

Field Coordinates::toField() const
{
   CheckValid(*this);
   return Row * NumCol + Col;
}

Coordinates Coordinates::fromField(Field f)
{
   Coordinates result;
   result.Row = f / NumCol;
   result.Col = f - result.Row * NumRowColBox;
   CheckValid(result);
   return result;
}
