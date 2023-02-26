#pragma once

#include "Mesh/IMesh.h"
#include "GridTopology.h"
#include "MultiIndex.h"
#include "MyException.h"

#include <span>

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

   // Return CellIndexInvalied if not found
   // Return first in lexicographical order if multiple matches
   CellIndex locatePointInCell(const Point<T, N>&) const;

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
   str::transform(coordinates, dimensions.begin(), [](const auto& d) {return static_cast<int>(d.size() - 1); });
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
Point<T, N> TartanGrid<T, N>::getPoint(PointIndex pointIndex) const
{
   const auto mp = m_multiIndexPoint.toMultiplet(pointIndex);
   Point<T, N> result;
   for (int n = 0; n < N; ++n)
   {
      result[n] = m_coordinates.at(n).at(mp.at(n));
   }
   return result;
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

template<typename T, int N>
CellIndex TartanGrid<T, N>::locatePointInCell(const Point<T, N>& point) const
{
   std::array<PointIndex, N> position;
   for (int n = 0; n < N; ++n)
   {
      if (point[n] < m_coordinates.at(n).front()) return PointIndexInvalid;
      if (point[n] > m_coordinates.at(n).back()) return PointIndexInvalid;
      if (point[n] == m_coordinates.at(n).front())
      {
         position[n] = 0;
      }
      else
      {
         const auto found = std::lower_bound(m_coordinates.at(n).begin(), m_coordinates.at(n).end(), point[n]);
         position[n] = std::distance(m_coordinates.at(n).begin(), found) - 1;
      }
   }
   return m_multiIndexCell.toFlat(position);
}