#include "BoundsCheck.h"
#include "GridTopology.h"
#include "MultiIndex.h"
#include "MyAssert.h"
#include "ReferenceShapeHyperCube.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"

using namespace Topology;

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
   const auto numCornersInCell = m_shape.getAdjacencies().getCountSafe(Corner);
   const auto cornerInCellIndexer = MultiIndex<int>::Create(std::vector<int>(maxdim, 2));
   std::vector<int> cellMultiplet(cellDimensions.size());
   std::vector<int> crnrMultiplet(maxdim);
   for (int cellId = 0; cellId < numCells; ++cellId)
   {
      cellIndexer.toMultiplet(cellId, cellMultiplet);
      cellToCorner.emplace(cellId, std::vector<int>{});
      for (int crnr = 0; crnr < numCornersInCell; ++crnr)
      {
         cornerInCellIndexer.toMultiplet(crnr, crnrMultiplet);
         str::transform(cellMultiplet, crnrMultiplet, cellMultiplet.begin(), std::plus());
         const auto crnrId = static_cast<int>(cornerIndexer.toFlat(cellMultiplet));
         // Restore
         str::transform(cellMultiplet, crnrMultiplet, cellMultiplet.begin(), std::minus());
         cellToCorner.at(cellId).push_back(crnrId);
      }
   }

   std::map<TopologyDimension, int> counts{
      {Corner, static_cast<int>(cornerIndexer.getFlatSize())},
      { m_shape.getAdjacencies().getMaxTopologyDimension(),static_cast<int>(numCells)} };
   std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacenciesList;
   adjacenciesList.emplace_back(TopologicalAdjacency::Create(maxdim, static_cast<int>(numCells), Corner, static_cast<int>(numCorners), cellToCorner));
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