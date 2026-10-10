#include "Cell.h"
#include "MyAssert.h"

#include <vector>

using namespace Sudoku;
using namespace Utilities;

namespace
{

Index Field2Row(Field field)
{
   return field / NumIndex;
}

Index Field2Col(Field field)
{
   return field - Field2Row(field) * NumIndex;
}

} // namespace

Cell::Cell(Field f)

{
   MyAssert(f < NumField);
   field = f;
   row = Field2Row(field);
   col = Field2Col(field);

   constexpr Index boxSize = 3;
   const Index boxRow = row / boxSize;
   const Index boxCol = col / boxSize;
   box = boxRow * boxSize + boxCol;

   MyAssert(row < NumIndex);
   MyAssert(col < NumIndex);
   MyAssert(box < NumIndex);
}

const Cell &Cell::get(Field f)
{
   static std::vector<Cell> s_allCells;
   if (s_allCells.empty())
   {
      for (Field f : FieldAll)
      {
         s_allCells.emplace_back(f);
      }
   }
   return s_allCells.at(f);
}

const Cell &Cell::getFromRowCol(Index row, Index col)
{
   const Field field = rowCol2Field(row, col);
   return get(field);
}

Field Cell::rowCol2Field(Index row, Index col)
{
   return row * NumIndex + col;
}
