#include "Ball2AsRegion.h"
#include "MyException.h"

using namespace Geometry;

template class Ball2AsRegion<double>;

namespace
{
template <typename T> class BallManifolds : public IRegionManifolds<T, 2>
{
  public:
    explicit BallManifolds(Sphere2AsManifold1<T> sphereManifold) : m_sphereManifold(std::move(sphereManifold))
    {
    }
    std::vector<const IManifold<T, GeomDim2> *> getAllManifolds() const override
    {
        return {&m_sphereManifold};
    }

    // Ignore lower dimensional manifolds
    std::vector<const IManifold<T, GeomDim2> *> getBoundaryHyperManifolds() const override
    {
        return getAllManifolds();
    }

    // Adjacencies
    std::vector<const IManifold<T, GeomDim2> *> getConnectedLowers(
        const IManifold<T, GeomDim2> &manifold) const override
    {
        return std::vector<const IManifold<T, GeomDim2> *>{};
    }
    std::vector<const IManifold<T, GeomDim2> *> getConnectedHighers(
        const IManifold<T, GeomDim2> &manifold) const override
    {
        return std::vector<const IManifold<T, GeomDim2> *>{};
    }

  private:
    const Sphere2AsManifold1<T> m_sphereManifold;
};
} // namespace

template <typename T>
Ball2AsRegion<T>::Ball2AsRegion(Ball<T, GeomDim2> ball, std::string name)
    : m_ball(std::move(ball)), m_name(std::move(name)),
      m_sphereManifolds(std::make_unique<BallManifolds<T>>(
          Sphere2AsManifold1<T>(Sphere<T, GeomDim2>(m_ball.getCenter(), m_ball.getRadius()))))
{
}

template <typename T> BoundingBox<T, GeomDim2> Ball2AsRegion<T>::getBoundingBox() const
{
    return m_ball.getBoundingBox();
}

template <typename T>
bool Ball2AsRegion<T>::contains(const Point<T, GeomDim2> &point, const IGeometryPredicate<T, GeomDim2> &predicate) const
{
    return m_ball.Contains(point, predicate);
}

template <typename T>
bool Ball2AsRegion<T>::couldIntersectWith(const BoundingBox<T, GeomDim2> &bb,
                                          const IGeometryPredicate<T, GeomDim2> &predicate) const
{
    return m_ball.CouldIntersectWith(bb, predicate);
}

template <typename T> const IRegionManifolds<T, GeomDim2> &Ball2AsRegion<T>::getManifolds() const
{
    return *m_sphereManifolds;
}

template <typename T> const std::string &Ball2AsRegion<T>::getName() const
{
    return m_name;
}
