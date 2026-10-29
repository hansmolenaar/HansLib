#include "SweepIntersectionElimination.h"
#include "House.h"
#include "MyAssert.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"

#include <optional>

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

std::optional<HouseId> SweepIntersectionElimination::TryGetIntersectionHouse(HouseType houseTypeMaster,
                                                                             std::span<const Field> fields)
{
   const auto size = fields.size();
   MyAssert(size >= 2 && size <= 3);

   const Cell &cell0 = Cell::get(fields[0]);
   const Cell &cell1 = Cell::get(fields[1]);
   if (houseTypeMaster == HouseType::Box)
   {
      // Row or Col
      if (cell0.row == cell1.row)
      {
         for (auto f : fields)
         {
            if (Cell::get(f).row != cell0.row)
            {
               return {};
            }
         }

         return HouseId{HouseType::Row, cell0.row};
      }
      else if (cell0.col == cell1.col)
      {
         for (auto f : fields)
         {
            if (Cell::get(f).col != cell0.col)
            {
               return {};
            }
         }

         return HouseId{HouseType::Col, cell0.col};
      }
   }
   else
   {
      for (auto f : fields)
      {
         if (Cell::get(f).box != cell0.box)
         {
            return {};
         }
      }

      return HouseId{HouseType::Box, cell0.box};
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
