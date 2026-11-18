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

FieldValue::Container SweepSimpleColoring::collectSees2ColorsCandidates(const FieldsInfo &finfo, Value value,
                                                                        const ColorField &colors,
                                                                        const FieldBits &cellsToIgnore)
{
   const auto cellsGreen = colors.getFieldsWithColor(Color::Green);
   const auto cellsBlue = colors.getFieldsWithColor(Color::Blue);

   const auto seesBlue = finfo.getAllVisibleFields(cellsBlue);
   const auto seesGreen = finfo.getAllVisibleFields(cellsGreen);
   auto fields = seesBlue & seesGreen;
   fields &= ~cellsToIgnore;
   fields &= finfo.getFieldsWithValue(value);

   FieldValue::Container result;
   FieldValue::add(fields, value, result);
   return result;
}

FieldValue::Container SweepSimpleColoring::collectTwiceInAUnit(const FieldsInfo &finfo, Value value,
                                                               const ColorField &colors)
{
   FieldValue::Container result;
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
            FieldValue::add(fieldsWithColor, value, result);
         }
         prev = f;
      }
   }
   return result;
}

FieldValue::Container SweepSimpleColoring::collectOneColorEmptiesHouse(const FieldsInfo &finfo, Value value,
                                                                       const ColorField &colors,
                                                                       const FieldBits &cellsToIgnore)
{
   FieldValue::Container result;
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
         FieldValue::add(fieldsBlue, value, result);
      }
      else if (numCellsInHouseSeesGreen == numCellsInHouseWithValue)
      {
         FieldValue::add(fieldsGreen, value, result);
      }
   }

   return result;
}

FieldValue::Container SweepSimpleColoring::collect(const FieldsInfo &finfo)
{
   FieldValue::Container result;
   const CandidatesGrid &grid = finfo.getGrid();
   for (Value value : ValueAll)
   {
      const auto links = getBilocationLinks(grid, finfo, value);

      if (links.empty())
      {
         continue;
      }
      const auto networks = Network::create(links);
      for (const auto &network : networks)
      {
         const auto coloring = Network::setColors(network);
         const auto cellsInCurrectNetwork = network.getFields();

         const auto twoColors =
             SweepSimpleColoring::collectSees2ColorsCandidates(finfo, value, coloring, cellsInCurrectNetwork);
         result.insert(twoColors.begin(), twoColors.end());

         const auto twice = SweepSimpleColoring::collectTwiceInAUnit(finfo, value, coloring);
         result.insert(twice.begin(), twice.end());

         const auto empties =
             SweepSimpleColoring::collectOneColorEmptiesHouse(finfo, value, coloring, cellsInCurrectNetwork);
         result.insert(empties.begin(), empties.end());
      }
   }

   return result;
}
