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

private:
   static MultiIndex<PointIndex> CreateMultiIndex(const std::vector<std::vector<T>>& coordinates);
   static std::unique_ptr<GridTopology> CreateTopology(const std::vector<std::vector<T>>& coordinates);
   std::vector<std::vector<T>> m_coordinates;
   MultiIndex<PointIndex> m_multiIndex;
   std::unique_ptr<GridTopology> m_topology;
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
std::unique_ptr<GridTopology> TartanGrid<T, N>::CreateTopology(const std::vector<std::vector<T>>& coordinates)
{
   std::vector<int> dimensions(coordinates.size());
   str::transform(coordinates, dimensions.begin(), [](const auto& d) {return static_cast<int>(d.size()); });
   return std::make_unique<GridTopology>(dimensions);
}


template<typename T, int N>
TartanGrid<T, N>::TartanGrid(std::vector<std::vector<T>>&& coordinates) :
   m_coordinates(std::move(coordinates)),
   m_multiIndex(CreateMultiIndex(m_coordinates)),
   m_topology(CreateTopology(m_coordinates))
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
   return m_multiIndex.getFlatSize();
}