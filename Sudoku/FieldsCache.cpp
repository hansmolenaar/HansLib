
#include "FieldsCache.h"
#include "House.h"
#include "SudokuDefines.h"

using namespace Sudoku;

FieldsCache::FieldsCache(const CandidatesGrid &grid) : m_grid(&grid)
{
   auto itr = m_fieldsInHouse.begin();
   for (const auto &house : House::getAllHouses())
   {
      *itr = SudokuUtils::getFields(house.cells);
      ++itr;
   }
}

FieldBits FieldsCache::getHouseFields(HouseId houseId)
{
   const int houseType = static_cast<int>(houseId.houseType);
   const auto pos = houseType * NumIndex + houseId.houseIndex;
   return m_fieldsInHouse.at(pos);
}
