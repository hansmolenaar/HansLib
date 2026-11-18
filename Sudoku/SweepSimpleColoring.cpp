#include "SweepSimpleColoring.h"
#include "CandidatesGrid.h"
#include "ColorField.h"
#include "FieldsInfo.h"
#include "House.h"
#include "MyAssert.h"
#include "Network.h"
#include "StreamUtils.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include "Visibility.h"

#include <algorithm>
#include <string>

using namespace Sudoku;
using namespace Utilities;

std::vector<Link> SweepSimpleColoring::getBilocationLinks(const CandidatesGrid &grid, FieldsInfo finfo, Value value)
{
   finfo.update();
   std::vector<Link> result;
   for (const House &house : House::getAllHouses())
   {
      const auto houseBits = finfo.getHouseFields(house.houseId);
      const auto fieldBits = houseBits & finfo.getActiveFieldsWithValue(value);

      if (fieldBits.count() == 2)
      {
         std::array<Field, 2> fieldPair;
         FieldsInfo::BitsToFields(fieldBits, fieldPair.begin());
         result.emplace_back(Cell::get(fieldPair[0]), Cell::get(fieldPair[1]), LinkType::Strong);
      }
   }

   UniqueSorted(result);
   return result;
}

bool SweepSimpleColoring::removeSees2ColorsCandidates(CandidatesGrid &grid, const FieldsInfo &finfo, Value value,
                                                      const ColorField &colors, const FieldBits &cellsToIgnore)

{
   const auto cellsGreen = colors.getFieldsWithColor(Color::Green);
   const auto cellsBlue = colors.getFieldsWithColor(Color::Blue);

   const auto seesBlue = finfo.getAllVisibleFields(cellsBlue);
   const auto seesGreen = finfo.getAllVisibleFields(cellsGreen);
   auto useFields = seesBlue & seesGreen;
   useFields &= ~cellsToIgnore;
   const bool anyChange = grid.removeCandidateFromFields(useFields, value);
   return anyChange;
}

bool SweepSimpleColoring::removeTwiceInAUnit(CandidatesGrid &grid, Value value, const ColorField &colors)
{
   for (Color color : ColorAll)
   {
      const auto fieldsWithColor = colors.getFieldsWithColor(color);
      const auto numFieldsWithColor = fieldsWithColor.count();
      if (numFieldsWithColor < 2)
      {
         continue;
      }

      std::optional<Field> prev;
      for (Field f : SudokuUtils::FieldBitsToRange(fieldsWithColor))
      {
         if (!prev.has_value())
         {
            prev = f;
            continue;
         }
         if (Visiblity::areVisible(f, prev.value()))
         {
            if (grid.removeCandidateFromFields(fieldsWithColor, value))
            {
               return true;
            }
         }
         prev = f;
      }
   }
   return false;
}

bool SweepSimpleColoring::removeOneColorEmptiesHouse(CandidatesGrid &grid, Value value, const ColorField &colors,
                                                     const FieldBits &cellsToIgnore)
{
   FieldsInfo finfo(grid);
   const FieldBits fieldsValue = finfo.getActiveFieldsWithValue(value);
   const auto fieldsBlue = colors.getFieldsWithColor(Color::Blue);
   const auto fieldsGreen = colors.getFieldsWithColor(Color::Green);
   const auto visibleFieldsBlue = finfo.getAllVisibleFields(fieldsBlue);
   const auto visibleFieldsGreen = finfo.getAllVisibleFields(fieldsGreen);
   for (const House &house : House::getAllHouses())
   {
      const auto &fieldsInHouse = finfo.getHouseFields(house.houseId);
      if ((fieldsInHouse & cellsToIgnore).any())
      {
         // House contains part of the chain, we can ignore it
         continue;
      }

      const auto cellsInHouseWithValue = fieldsValue & fieldsInHouse;
      const auto numCellsInHouseWithValue = cellsInHouseWithValue.count();
      if (numCellsInHouseWithValue == 0)
      {
         continue;
      }
      const auto numCellsInHouseSeesBlue = (cellsInHouseWithValue & visibleFieldsBlue).count();
      const auto numCellsInHouseSeesGreen = (cellsInHouseWithValue & visibleFieldsGreen).count();
      if (std::min(numCellsInHouseSeesBlue, numCellsInHouseSeesGreen) > 0)
      {
         // Both colors are visible, ready
         continue;
      }
      if (numCellsInHouseSeesBlue == numCellsInHouseWithValue)
      {
         if (grid.removeCandidateFromFields(fieldsBlue, value))
         {
            return true;
         }
      }
      else if (numCellsInHouseSeesGreen == numCellsInHouseWithValue)
      {
         if (grid.removeCandidateFromFields(fieldsGreen, value))
         {
            return true;
         }
      }
   }
   return false;
}

bool SweepSimpleColoring::sweep(CandidatesGrid &grid, Value value)
{
   const FieldsInfo finfo(grid);
   const auto links = getBilocationLinks(grid, finfo, value);
   if (links.empty())
   {
      return false;
   }
   const auto networks = Network::create(links);
   for (const auto &network : networks)
   {
      const auto coloring = Network::setColors(network);
      auto cellsInCurrectNetwork = finfo.getSolvedFields();
      for (auto field : SudokuUtils::FieldBitsToRange(network.getFields()))
      {
         cellsInCurrectNetwork.set(field);
      }

      if (SweepSimpleColoring::removeSees2ColorsCandidates(grid, finfo, value, coloring, cellsInCurrectNetwork))
      {
         // Better start from scratch
         return true;
      }
      if (SweepSimpleColoring::removeTwiceInAUnit(grid, value, coloring))
      {
         return true;
      }
      if (SweepSimpleColoring::removeOneColorEmptiesHouse(grid, value, coloring, cellsInCurrectNetwork))
      {
         MyAssert(false, "RemoveOneColorEmptiesHouse first time, not tested!!"); // TODO
         return true;
      }
   }

   return false;
}

bool SweepSimpleColoring::sweep(CandidatesGrid &grid)
{
   bool anyChange = false;
   for (Value value : ValueAll)
   {
      if (sweep(grid, value))
      {
         anyChange = true;
         grid.update();
      }
   }
   return anyChange;
}
