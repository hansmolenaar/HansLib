#include "StrategySimpleColoring.h"
#include "CandidatesGrid.h"
#include "ColorField.h"
#include "FieldsInfo.h"
#include "House.h"
#include "Network.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"
#include "UniqueSorted.h"
#include "Visibility.h"

#include <algorithm>

using namespace Sudoku;
using namespace Utilities;

std::vector<Link> StrategySimpleColoring::getBilocationLinks(const CandidatesGrid &grid, FieldsInfo finfo, Value value)
{
   std::vector<Link> result;
   for (const House &house : House::getAllHouses())
   {
      const auto houseBits = finfo.getHouseFields(house);
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

void StrategySimpleColoring::collectSees2ColorsCandidates(const FieldsInfo &finfo, Value value,
                                                          const ColorField &colors, const FieldBits &cellsToIgnore,
                                                          FieldValues &fieldValues)
{
   const auto cellsGreen = colors.getFieldsWithColor(Color::Green);
   const auto cellsBlue = colors.getFieldsWithColor(Color::Blue);

   const auto seesBlue = finfo.getAllVisibleFields(cellsBlue);
   const auto seesGreen = finfo.getAllVisibleFields(cellsGreen);
   auto fields = seesBlue & seesGreen;
   fields &= ~cellsToIgnore;
   fields &= finfo.getFieldsWithValue(value);

   fieldValues.add(fields, value);
}

void StrategySimpleColoring::collectTwiceInAUnit(const FieldsInfo &finfo, Value value, const ColorField &colors,
                                                 FieldValues &fieldValues)
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
         if (Visibility::areVisible(f, prev.value()))
         {
            fieldValues.add(fieldsWithColor, value);
         }
         prev = f;
      }
   }
}

void StrategySimpleColoring::collectOneColorEmptiesHouse(const FieldsInfo &finfo, Value value, const ColorField &colors,
                                                         const FieldBits &cellsToIgnore, FieldValues &fieldValues)
{
   const FieldBits fieldsValue = finfo.getActiveFieldsWithValue(value);
   const auto fieldsBlue = colors.getFieldsWithColor(Color::Blue);
   const auto fieldsGreen = colors.getFieldsWithColor(Color::Green);
   const auto visibleFieldsBlue = finfo.getAllVisibleFields(fieldsBlue);
   const auto visibleFieldsGreen = finfo.getAllVisibleFields(fieldsGreen);
   for (const House &house : House::getAllHouses())
   {
      const auto &fieldsInHouse = finfo.getHouseFields(house);
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
         fieldValues.add(fieldsBlue, value);
      }
      else if (numCellsInHouseSeesGreen == numCellsInHouseWithValue)
      {
         fieldValues.add(fieldsGreen, value);
      }
   }
}

std::string StrategySimpleColoring::getName() const
{
   return "Simple coloring";
}
void StrategySimpleColoring::collect(const FieldsInfo &finfo, FieldValues &result)
{
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

         StrategySimpleColoring::collectSees2ColorsCandidates(finfo, value, coloring, cellsInCurrectNetwork, result);
         StrategySimpleColoring::collectTwiceInAUnit(finfo, value, coloring, result);
         StrategySimpleColoring::collectOneColorEmptiesHouse(finfo, value, coloring, cellsInCurrectNetwork, result);
      }
   }
}
