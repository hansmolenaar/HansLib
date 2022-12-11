#include "GridTopology.h"
#include "ReferenceShapeHyperCube.h"
#include "Utilities/Assert.h"
#include "Utilities/BoundsCheck.h"
#include "Utilities/Defines.h"
#include "Utilities/MultiIndex.h"
#include "TopologicalAdjacencies.h"
#include "TopologicalAdjacency.h"

GridTopology::GridTopology(const std::vector<int>& cellDimensions) :
   m_shape(ReferenceShapeHyperCube::Get(static_cast<TopologyDimension>(cellDimensions.size())))
{
   Utilities::Assert(!cellDimensions.empty());
   Utilities::Assert(std::ranges::all_of(cellDimensions, BoundsCheck<int>::CreateIsPositive()));
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
         size_t pos = 0;
         for (auto c : crnrMultiplet)
         {
            cellMultiplet.at(pos) += crnrMultiplet.at(pos);
            const auto crnrId = cornerIndexer.toFlat(cellMultiplet);
            cellMultiplet.at(pos) -= crnrMultiplet.at(pos);
            cellToCorner.at(cellId).push_back(crnrId);
         }
      }
   }
   std::map<TopologyDimension, int> counts{
      {TopologyDimensionDef::Corner, cornerIndexer.getFlatSize()},
      { m_shape.getAdjacencies().getMaxTopologyDimension(),numCells} };
   std::vector<std::unique_ptr<ITopologicalAdjacency>> adjacenciesList;
   adjacenciesList.emplace_back(TopologicalAdjacency::Create(maxdim, numCells, TopologyDimensionDef::Corner, numCorners, cellToCorner));
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