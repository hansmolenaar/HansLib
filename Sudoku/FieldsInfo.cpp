
#include "FieldsInfo.h"
#include "BitsetUtils.h"
#include "House.h"
#include "SudokuDefines.h"

using namespace Sudoku;
using namespace Utilities;

namespace
{
Field getFlatHouseId(HouseType houseType, Index index)
{
   const int flatType = static_cast<int>(houseType);
   const auto pos = flatType * NumIndex + index;
   return pos;
}

} // namespace

FieldsInfo::FieldsInfo(const CandidatesGrid &grid) : m_grid(&grid)
{
   // Fields in House
   for (Field field : FieldAll)
   {
      const Cell cell = Cell::get(field);
      m_fieldsInHouse[getFlatHouseId(HouseType::Row, cell.row)].set(field);
      m_fieldsInHouse[getFlatHouseId(HouseType::Col, cell.col)].set(field);
      m_fieldsInHouse[getFlatHouseId(HouseType::Box, cell.box)].set(field);
   }

   // Visibility
   for (Field field : FieldAll)
   {
      const Cell cell = Cell::get(field);
      m_visible[field] |= getHouseFields({HouseType::Row, cell.row});
      m_visible[field] |= getHouseFields({HouseType::Col, cell.col});
      m_visible[field] |= getHouseFields({HouseType::Box, cell.box});
      // By convention
      m_visible[field].reset(field);
   }

   update();
}

void FieldsInfo::update()
{
   // Clear all mutable data and ...
   m_solvedFields.reset();
   m_activeFields.reset();
   for (Value value : ValueAll)
   {
      m_activeFieldswithValue[value].reset();
      m_fieldswithValue[value].reset();
   }

   // ... refill
   for (Field field : FieldAll)
   {
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

      for (Value value : candidates)
      {
         m_fieldswithValue[value].set(field);
      }
   }
}

FieldBits FieldsInfo::getVisibleFields(Field field) const
{
   return m_visible.at(field);
}

FieldBits FieldsInfo::getAllVisibleFields(const FieldBits &fieldBits) const
{
   FieldBits result;
   for (Field field : FieldAll | std::views::filter(Bitset{fieldBits}))
   {
      result |= getVisibleFields(field);
   }
   return result;
}

FieldBits FieldsInfo::getHouseFields(const House &house) const
{
   const auto pos = getFlatHouseId(house.houseType, house.houseIndex);
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

FieldBits FieldsInfo::getFieldsWithValue(Value value) const
{
   return m_fieldswithValue.at(value);
}

const CandidatesGrid &FieldsInfo::getGrid() const
{
   return *m_grid;
}
