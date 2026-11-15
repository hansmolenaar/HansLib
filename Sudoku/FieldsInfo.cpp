
#include "FieldsInfo.h"
#include "House.h"
#include "SudokuDefines.h"

using namespace Sudoku;

namespace
{
Field getFlatHouseId(HouseType houseType, Index index)
{
   const int flatType = static_cast<int>(houseType);
   const auto pos = flatType * NumIndex + index;
   return pos;
}

Field getFlatHouseId(HouseId houseId)
{
   return getFlatHouseId(houseId.houseType, houseId.houseIndex);
}

} // namespace

FieldsInfo::FieldsInfo(const CandidatesGrid &grid) : m_grid(&grid)
{
   // Fields in House
   auto itr = m_fieldsInHouse.begin();
   for (const auto &house : House::getAllHouses())
   {
      *itr = SudokuUtils::getFields(house.cells);
      ++itr;
   }

   update();
}

void FieldsInfo::update()
{
   // Clear all mutable data and ...
   m_solvedFields.reset();
   m_activeFields.reset();
   for (auto itr : m_activeFieldswithValue)
   {
      itr.reset();
   }

   // ... refill
   for (const Cell &cell : Cell::getAll())
   {
      const Field field = cell.field;
      const Candidates &candidates = (*m_grid)[field];
      if (candidates.isSolved())
      {
         m_solvedFields.set(field);
      }
      else
      {
         m_activeFields.set(field);
         for (Value value : candidates)
         {
            m_activeFieldswithValue[value].set(field);
         }
      }
   }
}

FieldBits FieldsInfo::getHouseFields(HouseId houseId) const
{
   const auto pos = getFlatHouseId(houseId);
   return m_fieldsInHouse.at(pos);
}

FieldBits FieldsInfo::getActiveFieldsWithValue(Value value) const
{
   return m_activeFieldswithValue.at(value);
}

FieldBits FieldsInfo::getActiveFields() const
{
   return m_activeFields;
}

FieldBits FieldsInfo::getSolvedFields() const
{
   return m_solvedFields;
}
