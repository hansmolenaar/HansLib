#pragma once

#include "CandidatesGrid.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"

namespace Sudoku
{

struct HouseId;

class FieldsCache
{
 public:
   explicit FieldsCache(const CandidatesGrid &grid);
   FieldBits getHouseFields(HouseId);

 private:
   const CandidatesGrid *m_grid;
   std::array<FieldBits, NumHouse> m_fieldsInHouse;
};

} // namespace Sudoku
