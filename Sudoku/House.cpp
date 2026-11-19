#include "House.h"
#include "SudokuDefines.h"

#include <ostream>
#include <sstream>
#include <stdexcept>

using namespace Sudoku;

std::span<const House> House::getAllHouses()
{
   constexpr static std::array<House, NumHouse> s_allHouses = {
       House{HouseType::Row, 0}, House{HouseType::Row, 1}, House{HouseType::Row, 2},
       House{HouseType::Row, 3}, House{HouseType::Row, 4}, House{HouseType::Row, 5},
       House{HouseType::Row, 6}, House{HouseType::Row, 7}, House{HouseType::Row, 8},

       House{HouseType::Col, 0}, House{HouseType::Col, 1}, House{HouseType::Col, 2},
       House{HouseType::Col, 3}, House{HouseType::Col, 4}, House{HouseType::Col, 5},
       House{HouseType::Col, 6}, House{HouseType::Col, 7}, House{HouseType::Col, 8},

       House{HouseType::Box, 0}, House{HouseType::Box, 1}, House{HouseType::Box, 2},
       House{HouseType::Box, 3}, House{HouseType::Box, 4}, House{HouseType::Box, 5},
       House{HouseType::Box, 6}, House{HouseType::Box, 7}, House{HouseType::Box, 8}};

   return s_allHouses;
}

House House::getFromCellaAndType(const Cell &cell, HouseType type)
{
   switch (type)
   {
   case HouseType::Row:
      return {type, cell.row};
   case HouseType::Col:
      return {type, cell.col};
   case HouseType::Box:
      return {type, cell.box};
   default:
      throw std::runtime_error("Should not come here");
   }
}

std::string House::toString(HouseType houseType)
{
   if (houseType == HouseType::Row)
   {
      return "ROW";
   }
   else if (houseType == HouseType::Col)
   {
      return "COL";
   }
   else
   {
      return "BOX";
   }
}
std::string House::toString(const House &house)

{
   std::stringstream stream;
   stream << house;
   return stream.str();
}

std::ostream &operator<<(std::ostream &os, const Sudoku::House &house)
{
   os << "[" << House::toString(house.houseType) << " " << house.houseIndex << "]";
   return os;
}
