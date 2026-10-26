#include "Cell.h"
#include "MyAssert.h"

#include <algorithm>
#include <vector>

using namespace Sudoku;
using namespace Utilities;
namespace str = std::ranges;

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

const std::vector<Cell> &Cell::getAll()
{
   static std::vector<Cell> s_allCells;
   if (s_allCells.empty())
   {
      for (Field f : FieldAll)
      {
         s_allCells.emplace_back(f);
      }
   }
   return s_allCells;
}

const Cell &Cell::get(Field f)
{
   return getAll().at(f);
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

bool Cell::AreSame(const Cell &cell, Field field)
{
   return cell.field == field;
}

Field Cell::getField(const Cell &cell)
{
   return cell.field;
}

boost::container::static_vector<Field, NumField> Cell::cells2fields(std::span<const Cell> cells)
{
   boost::container::static_vector<Field, NumField> result(cells.size());
   str::transform(cells, result.begin(), Cell::getField);
   return result;
}
