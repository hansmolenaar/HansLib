#pragma once

#include "CandidatesGrid.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"

namespace Sudoku
{

struct HouseId;

class FieldsInfo
{
 public:
   explicit FieldsInfo(const CandidatesGrid &grid);
   void update();
   FieldBits getHouseFields(HouseId) const;
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
