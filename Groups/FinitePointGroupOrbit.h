#pragma once

#include "IFiniteGroupUtils.h"
#include "IFinitePointGroupAction.h"
#include "IUniquePointCollection.h"

template <size_t N> class FinitePointGroupOrbit : public IUniquePointCollection<double, N>
{
  public:
    FinitePointGroupOrbit(const IFinitePointGroupAction<N> &, const Point<double, N> &,
                          const IGeometryPredicate<double, N> &);
    Point<double, N> getPoint(PointIndex) const override;
    PointIndex getNumPoints() const override;
    std::optional<PointIndex> tryGetClosePoint(const Point<double, N> &) const override;
    const IGeometryPredicate<double, N> &getGeometryPredicate() const override;

  private:
    const IGeometryPredicate<double, N> &m_predicate;
    const IFinitePointGroupAction<N> &m_action;
    std::vector<Point<double, N>> m_uniquePoints;
};

template <size_t N>
FinitePointGroupOrbit<N>::FinitePointGroupOrbit(const IFinitePointGroupAction<N> &action, const Point<double, N> &point,
                                                const IGeometryPredicate<double, N> &predicate)
    : m_predicate(predicate), m_action(action)
{
    for (auto g : IFiniteGroupUtils::GetElements(m_action.getGroup()))
    {
        const auto image = m_action(g, point);
        const auto found = tryGetClosePoint(image);
        if (!found)
        {
            m_uniquePoints.push_back(image);
        }
    }
}

template <size_t N> const IGeometryPredicate<double, N> &FinitePointGroupOrbit<N>::getGeometryPredicate() const
{
    return m_predicate;
}

template <size_t N> Point<double, N> FinitePointGroupOrbit<N>::getPoint(PointIndex n) const
{
    return m_uniquePoints.at(n);
}

template <size_t N> PointIndex FinitePointGroupOrbit<N>::getNumPoints() const
{
    return static_cast<PointIndex>(m_uniquePoints.size());
}

template <size_t N>
std::optional<PointIndex> FinitePointGroupOrbit<N>::tryGetClosePoint(const Point<double, N> &p) const
{
    PointIndex result = PointIndexInvalid;
    for (const auto &up : m_uniquePoints)
    {
        result = result == PointIndexInvalid ? 0 : result + 1;
        if (m_predicate.samePoints(up, p))
            return result;
    }
    return {};
}
