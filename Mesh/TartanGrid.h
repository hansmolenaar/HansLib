#pragma once

#include "Mesh/IMesh.h"
#include "Topology/GridTopology.h"
#include "Utilities/MultiIndex.h"
#include "Utilities/MyException.h"

template<typename T, int N>
class TartanGrid : public IMesh<T, N>, IPointCollection<T, N>
{
public:
   explicit TartanGrid(std::vector<std::vector<T>>&& coordinates);

   const ITopologicalAdjacencies& getTopology() const override;
   const IPointCollection<T, N>& getGeometry() const override;

   Point<T, N> getPoint(PointIndex) const override;
   PointIndex getNumPoints() const override;

   const MultiIndex<PointIndex>& getPointIndexer() const;
   const MultiIndex<CellIndex>& getCellIndexer() const;

private:
   static MultiIndex<CellIndex> CreateCellIndexer(const MultiIndex<PointIndex>&);
   static MultiIndex<PointIndex> CreateMultiIndex(const std::vector<std::vector<T>>& coordinates);
   static std::unique_ptr<GridTopology> CreateTopology(const std::vector<std::vector<T>>& coordinates);

   std::vector<std::vector<T>> m_coordinates;
   std::unique_ptr<GridTopology> m_topology;
   MultiIndex<PointIndex> m_multiIndexPoint;
   MultiIndex<CellIndex> m_multiIndexCell;
   
};

template<typename T, int N>
MultiIndex<PointIndex> TartanGrid<T, N>::CreateMultiIndex(const std::vector<std::vector<T>>& coordinates)
{
   if (coordinates.size() != N) throw MyException("TartanGrid<T, N>::CreateMultiIndex inconsistent dimensions");
   std::vector<PointIndex> dimensions(coordinates.size());
   str::transform(coordinates, dimensions.begin(), [](const auto& d) {return d.size(); });
   return MultiIndex<PointIndex>::Create(std::move(dimensions));
}

template<typename T, int N>
MultiIndex<CellIndex> TartanGrid<T, N>::CreateCellIndexer(const MultiIndex<PointIndex>& pointIndexer)
{
   std::vector<size_t> cellDimensions;
   str::transform(std::views::iota(0, N), std::back_inserter(cellDimensions), [&pointIndexer](auto n) {return pointIndexer.at(n) - 1; });
   return MultiIndex<CellIndex>::Create(std::move(cellDimensions));
}


template<typename T, int N>
std::unique_ptr<GridTopology> TartanGrid<T, N>::CreateTopology(const std::vector<std::vector<T>>& coordinates)
{
   std::vector<int> dimensions(coordinates.size());
   str::transform(coordinates, dimensions.begin(), [](const auto& d) {return static_cast<int>(d.size()-1); });
   return std::make_unique<GridTopology>(dimensions);
}


template<typename T, int N>
TartanGrid<T, N>::TartanGrid(std::vector<std::vector<T>>&& coordinates) :
   m_coordinates(std::move(coordinates)),
   m_topology(CreateTopology(m_coordinates)),
   m_multiIndexPoint(CreateMultiIndex(m_coordinates)),
   m_multiIndexCell(CreateCellIndexer(m_multiIndexPoint))
   
{}

template<typename T, int N>
const ITopologicalAdjacencies& TartanGrid<T, N>::getTopology() const
{
   return m_topology->getAdjacencies();
}

template<typename T, int N>
const IPointCollection<T, N>& TartanGrid<T, N>::getGeometry() const
{
   return *this;
}

template<typename T, int N>
Point<T, N> TartanGrid<T, N>::getPoint(PointIndex) const
{
   throw MyException("TartanGrid<T, N>::getPoint(PointIndex) not implemented");
}

template<typename T, int N>
PointIndex TartanGrid<T, N>::getNumPoints() const
{
   return m_multiIndexPoint.getFlatSize();
}

template<typename T, int N>
const MultiIndex<PointIndex>& TartanGrid<T, N>::getPointIndexer() const
{
   return m_multiIndexPoint;
}

template<typename T, int N>
const MultiIndex<CellIndex>& TartanGrid<T, N>::getCellIndexer() const
{
   return m_multiIndexCell;
}