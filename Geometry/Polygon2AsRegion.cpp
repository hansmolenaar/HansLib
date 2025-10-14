#include "DirectedEdge.h"
#include "DirectedEdge.h"
#include "MyException.h"
#include "Polygon2AsRegion.h"
#include "Polygon2D.h"
#include "UnitVector.h"

using namespace Utilities;
using namespace Geometry;

template Polygon2AsRegion<double>;

template<typename T>
Polygon2AsRegion<T>::Polygon2AsRegion(std::span<const Point<T, 2>> polygon, std::string name) :
   m_polygon(polygon.size()),
   m_bb(BoundingBox<T, 2>::CreateFromList(polygon)),
   m_name(std::move(name))
{
   const auto numPoints = polygon.size();

   str::copy(polygon, m_polygon.begin());

   m_pointManifolds.reserve(numPoints);
   for (size_t n = 0; n < m_polygon.size(); ++n)
   {
      m_pointManifolds.emplace_back(m_polygon[n], m_name + "_point_" + std::to_string(n));
   }

   m_edgeManifolds.reserve(numPoints);
   for (size_t n = 0; n < numPoints; ++n)
   {
      const DirectedEdge<T, GeomDim2> edge(polygon[n], polygon[(n + 1) % numPoints]);
      const std::string name = m_name + "_edge_" + std::to_string(n);
      m_edgeManifolds.emplace_back(edge, name);
   }
}

template<typename T>
const std::string& Polygon2AsRegion<T>::getName() const
{
   return m_name;
}

template<typename T>
BoundingBox<T, GeomDim2> Polygon2AsRegion<T>::getBoundingBox() const
{
   return m_bb;
}

template<typename T>
bool Polygon2AsRegion<T>::contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   return Polygon2D::Contains<T>(m_polygon, point, predicate);
}

template<typename T>
const IRegionManifolds<T, GeomDim2>& Polygon2AsRegion<T>::getManifolds() const
{
   return *this;
}

template<typename T>
std::vector<const IManifold<T, GeomDim2>*> Polygon2AsRegion<T>::getBoundaryHyperManifolds() const
{
   std::vector<const IManifold<T, GeomDim2>*> result(m_edgeManifolds.size());
   str::transform(m_edgeManifolds, result.begin(), [](const auto& manifold) {return &manifold; });
   return result;
}

template<typename T>
std::vector<const IManifold<T, GeomDim2>*> Polygon2AsRegion<T>::getAllManifolds() const
{
   const auto numPoints = m_polygon.size();
   auto result = getBoundaryHyperManifolds();
   result.resize(2 * numPoints);
   str::transform(m_pointManifolds, result.begin() + numPoints, [](const auto& manifold) {return &manifold; });
   return result;
}

template<typename T>
std::vector<const IManifold<T, GeomDim2>*> Polygon2AsRegion<T>::getConnectedHighers(const IManifold<T, GeomDim2>& manifold) const
{
   const auto numPoints = m_polygon.size();
   const auto predicateManifold = [&manifold](const Manifold0<T, GeomDim2>& pointManifold) { return &pointManifold == &manifold; };
   const auto found = str::find_if(m_pointManifolds, predicateManifold);
   if (found == m_pointManifolds.end()) return {};
   const auto p = std::distance(m_pointManifolds.begin(), found);
   const auto e0 = p;
   const auto e1 = (p > 0) ? p - 1 : numPoints - 1;
   return { &m_edgeManifolds.at(e0), &m_edgeManifolds.at(e1) };
}

template<typename T>
std::vector<const IManifold<T, GeomDim2>*> Polygon2AsRegion<T>::getConnectedLowers(const IManifold<T, GeomDim2>& manifold) const
{
   const auto numPoints = m_polygon.size();
   const auto predicateManifold = [&manifold](const DirectedEdge2AsManifold1<T>& edgeManifold) { return &edgeManifold == &manifold; };
   const auto found = str::find_if(m_edgeManifolds, predicateManifold);
   if (found == m_edgeManifolds.end()) return {};
   const auto e = std::distance(m_edgeManifolds.begin(), found);
   const auto p0 = e;
   const auto p1 = (e + 1) % numPoints;
   return { &m_pointManifolds.at(p0), &m_pointManifolds.at(p1) };
}