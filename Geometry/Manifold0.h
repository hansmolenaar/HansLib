#pragma once
#include "IManifold0.h"

namespace Geometry
{

template <typename T, size_t N> class Manifold0 : public IManifold0<T, N>
{
  public:
    Manifold0(const Point<T, N> &point, std::string name);

    Point<T, N> getPoint() const override;
    bool contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const override;

    BoundingBox<T, N> getBoundingBox() const override;

    Topology::TopologyDimension getTopologyDimension() const override
    {
        return Topology::Corner;
    };

    const std::string &getName() const override;

  private:
    Point<T, N> m_point;
    std::string m_name;
};

template <typename T, size_t N>
Manifold0<T, N>::Manifold0(const Point<T, N> &point, std::string name) : m_point(point), m_name(std::move(name))
{
}

template <typename T, size_t N> Point<T, N> Manifold0<T, N>::getPoint() const
{
    return m_point;
}

template <typename T, size_t N> const std::string &Manifold0<T, N>::getName() const
{
    return m_name;
}

template <typename T, size_t N>
bool Manifold0<T, N>::contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const
{
    return predicate.samePoints(point, m_point);
}

template <typename T, size_t N> BoundingBox<T, N> Manifold0<T, N>::getBoundingBox() const
{
    return BoundingBox<T, N>::Create(m_point);
}
} // namespace Geometry
