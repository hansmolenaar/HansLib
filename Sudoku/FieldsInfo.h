#pragma once

#include "CandidatesGrid.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"

namespace Sudoku
{

struct House;
struct HouseId;

class FieldsInfo
{
 public:
   explicit FieldsInfo(const CandidatesGrid &grid);
   void update();
   const CandidatesGrid &getGrid() const;
   FieldBits getHouseFields(const House &) const;

 private:
   FieldBits getHouseFields(HouseId) const;

 public:
   FieldBits getFieldsWithValue(Value) const;
   FieldBits getActiveFieldsWithValue(Value) const;
   FieldBits getActiveFields() const;
   FieldBits getSolvedFields() const;
   FieldBits getVisibleFields(Field) const;
   FieldBits getAllVisibleFields(const FieldBits &) const;

   template <class OutputIt> static void BitsToFields(const FieldBits &fb, OutputIt itr);

 private:
   // Immutable
   const CandidatesGrid *m_grid;
   std::array<FieldBits, NumHouse> m_fieldsInHouse;
   std::array<FieldBits, NumField> m_visible;

   // Mutable
   std::array<FieldBits, NumValue + 1> m_activeFieldswithValue;
   std::array<FieldBits, NumValue + 1> m_fieldswithValue;
   FieldBits m_activeFields;
   FieldBits m_solvedFields;
};

template <class OutputIt> void FieldsInfo::BitsToFields(const FieldBits &fb, OutputIt itr)
{
   for (Field field : FieldAll)
   {
      if (fb[field])
      {
         *itr = field;
         ++itr;
      }
   }
}

} // namespace Sudoku
