
#include "FieldsInfo.h"
#include "House.h"
#include "SudokuDefines.h"

using namespace Sudoku;

FieldsInfo::FieldsInfo(const CandidatesGrid &grid) : m_grid(&grid)
{
   // Fields in House
   auto itr = m_fieldsInHouse.begin();
   for (const auto &house : House::getAllHouses())
   {
      *itr = SudokuUtils::getFields(house.cells);
      ++itr;
   }

   // Nonsolved fields with value
   for (const Cell &cell : Cell::getAll())
   {
      const auto &candidates = grid[cell.field];
      if (!candidates.isSolved())
      {
         for (Value value : candidates)
         {
            m_activeFieldswithValue[value].set(cell.field);
         }
      }
   }
}

FieldBits FieldsInfo::getHouseFields(HouseId houseId)
{
   const int houseType = static_cast<int>(houseId.houseType);
   const auto pos = houseType * NumIndex + houseId.houseIndex;
   return m_fieldsInHouse.at(pos);
}

FieldBits FieldsInfo::getActiveFieldsWithValue(Value value)
{
   return m_activeFieldswithValue.at(value);
}
