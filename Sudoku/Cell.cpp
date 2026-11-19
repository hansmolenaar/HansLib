#include "Cell.h"
#include "MyAssert.h"

#include <ostream>
#include <stdexcept>
#include <string>
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

Index Cell::operator[](HouseType type) const
{
   switch (type)
   {
   case HouseType::Row:
      return row;
   case HouseType::Col:
      return col;
   case HouseType::Box:
      return box;
   default:
      throw std::runtime_error("Cell::operator[] unknown HouseType");
   }
}

std::span<const Cell> Cell::getAll()
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
   return getAll()[f];
}

const Cell &Cell::getFromRowCol(Index row, Index col)
{
   const Field field = rowCol2Field(row, col);
   return get(field);
}

Field Cell::rowCol2Field(Index row, Index col)
{
   MyAssert(row >= 0 && row < NumIndex && col >= 0 && col < NumIndex,
            "Cell::rowCol2Field invalid row,col: (" + std::to_string(row) + ", " + std::to_string(col) + ")");
   return row * NumIndex + col;
}

std::ostream &operator<<(std::ostream &os, const Sudoku::Cell &cell)
{
   // os << "(" << cell.row << ", " << cell.col << ", {" << cell.box << "})";
   os << "(r" << cell.row << ", c" << cell.col << ", b" << cell.box << " f" << cell.field << ")";
   return os;
}

// Rotate over 90 deg
const Cell &Cell::Rotate(const Cell &cell)
{
   const int rowIn = cell.row - 4;
   const int colIn = cell.col - 4;
   const int rowOut = colIn;
   const int colOut = -rowIn;
   return getFromRowCol(rowOut + 4, colOut + 4);
}

const Cell &Cell::ReflectX(const Cell &cell)
{
   const int rowIn = cell.row - 4;
   const int rowOut = -rowIn;
   return getFromRowCol(rowOut + 4, cell.col);
}

const Cell &Cell::ReflectY(const Cell &cell)
{
   const int colIn = cell.col - 4;
   const int colOut = -colIn;
   return getFromRowCol(cell.row, colOut + 4);
}

const Cell &Cell::ReflectYX(const Cell &cell)
{
   const int rowIn = cell.row - 4;
   const int colIn = cell.col - 4;
   return getFromRowCol(colIn + 4, rowIn + 4);
}

const Cell &Cell::ReflectXY(const Cell &cell)
{
   const int rowIn = cell.row - 4;
   const int colIn = cell.col - 4;
   return getFromRowCol(-colIn + 4, -rowIn + 4);
}
