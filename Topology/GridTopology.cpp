#include "GridTopology.h"
#include "ReferenceShapeHyperCube.h"
#include "MyAssert.h"
#include "BoundsCheck.h"
#include "Defines.h"
#include "MultiIndex.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"

GridTopology::GridTopology(const std::vector<int>& cellDimensions) :
   m_shape(ReferenceShapeHyperCube::Get(static_cast<TopologyDimension>(cellDimensions.size())))
{
   Utilities::MyAssert(!cellDimensions.empty());
   Utilities::MyAssert(std::ranges::all_of(cellDimensions, BoundsCheck<int>::CreateIsPositive()));
   const TopologyDimension maxdim = m_shape.getAdjacencies().getMaxTopologyDimension();
   const auto cellIndexer = MultiIndex<int>::Create(std::vector<int>(cellDimensions));
   const auto numCells = cellIndexer.getFlatSize();
   std::vector<int> crnrDimension(cellDimensions);
   std::ranges::transform(crnrDimension, crnrDimension.begin(), [](const auto& d) {return d + 1; });
   const auto cornerIndexer = MultiIndex<int>::Create(std::vector<int>(crnrDimension));
   const auto numCorners = cornerIndexer.getFlatSize();
   std::map<int, std::vector<int>> cellToCorner;
   const auto numCornersInCell = m_shape.getAdjacencies().getCountSafe(TopologyDimensionDef::Corner);
   const auto cornerInCellIndexer = MultiIndex<int>::Create(std::vector<int>(maxdim, 2));
   for (int cellId = 0; cellId < numCells; ++cellId)
   {
      auto cellMultiplet = cellIndexer.toMultiplet(cellId);
      cellToCorner.emplace(cellId, std::vector<int>{});
      for (int crnr = 0; crnr < numCornersInCell; ++crnr)
      {
         const auto crnrMultiplet = cornerInCellIndexer.toMultiplet(crnr);
         str::transform(cellMultiplet, crnrMultiplet, cellMultiplet.begin(), std::plus());
         const auto crnrId = static_cast<int>(cornerIndexer.toFlat(cellMultiplet));
         // Restore
         str::transform(cellMultiplet, crnrMultiplet, cellMultiplet.begin(), std::minus());
         cellToCorner.at(cellId).push_back(crnrId);
      }
   }

   std::map<TopologyDimension, int> counts{
      {TopologyDimensionDef::Corner, static_cast<int>(cornerIndexer.getFlatSize())},
      { m_shape.getAdjacencies().getMaxTopologyDimension(),static_cast<int>(numCells)} };
   std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacenciesList;
   adjacenciesList.emplace_back(TopologicalAdjacency::Create(maxdim, static_cast<int>(numCells), TopologyDimensionDef::Corner, static_cast<int>(numCorners), cellToCorner));
   m_adjacencies = TopologicalAdjacencies::CreateWithPartialCounts(maxdim, std::move(counts), std::move(adjacenciesList));
}

const ITopologicalAdjacencies& GridTopology::getAdjacencies() const
{
   return *m_adjacencies;
}

const IReferenceShapeHyperCube& GridTopology::getShape() const
{
   return m_shape;
}