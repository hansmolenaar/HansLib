#include "SweepIntersectionElimination.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "boost/container/static_vector.hpp"

#include <algorithm>
#include <optional>

namespace str = std::ranges;
using namespace Sudoku;
using namespace Utilities;

namespace
{

bool SweepHouse(CandidatesGrid &grid, const House &house)
{
   bool anyChange = false;
   const auto value2fields = SudokuUtils::getValue2Fields(grid, house.cells);
   for (Value value : ValueAll)
   {
      const auto numFieldsWithValue = value2fields[value].size();
      if (numFieldsWithValue >= 2 && numFieldsWithValue <= 3)
      {
      }
   }
   return anyChange;
};

} // namespace

std::optional<HouseId> SweepIntersectionElimination::TryGetIntersectionHouse(HouseType houseTypeIn,
                                                                             std::span<const Field> fields)
{
   const auto size = fields.size();
   MyAssert(size >= 2 && size <= 3);
   boost::container::static_vector<Cell, 3> cells(size);
   str::transform(fields, cells.begin(), Cell::get);
   const HouseId houseIn = HouseId::getFromCellaAndType(cells.front(), houseTypeIn);
   MyAssert(str::all_of(cells, CellInHouse{houseIn}));

   HouseId houseOut;
   if (houseTypeIn == HouseType::Box)
   {
      // Row or Col
      houseOut = HouseId::getFromCellaAndType(cells.front(), HouseType::Row);
      if (str::all_of(cells, CellInHouse{houseOut}))
      {
         return houseOut;
      }
      houseOut = HouseId::getFromCellaAndType(cells.front(), HouseType::Col);
      if (str::all_of(cells, CellInHouse{houseOut}))
      {
         return houseOut;
      }
   }
   else
   {
      // Only box possible
      houseOut = HouseId::getFromCellaAndType(cells.front(), HouseType::Box);
      if (str::all_of(cells, CellInHouse{houseOut}))
      {
         return houseOut;
      }
   }

   return {};
}

bool SweepIntersectionElimination::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (const auto &house : House::getAllHouses())
   {
      anyChange |= SweepHouse(grid, house);
   }
   return anyChange;
};
