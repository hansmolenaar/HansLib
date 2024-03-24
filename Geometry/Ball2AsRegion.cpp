#include "Ball2AsRegion.h"
#include "MyException.h"

using namespace Geometry;

template Ball2AsRegion<double>;

namespace
{
   template<typename T>
   class BallManifolds : public IRegionManifolds<T, 2>
   {
   public:
      explicit BallManifolds(Sphere2AsManifold1<T> sphereManifold) : m_sphereManifold(std::move(sphereManifold)) {}
      std::vector<const IManifold<T, 2>*> GetAllManifolds() const override { return { &m_sphereManifold }; }

      // Ignore lower dimensional manifolds
      std::vector<const IManifold<T, 2>*> GetBoundaryHyperManifolds() const override { return GetAllManifolds(); }

      // Adjacencies
      std::vector<const IManifold<T, 2>*> GetConnectedLowers(const IManifold<T, 2>& manifold) const override 
      { 
         return std::vector<const IManifold<T, 2>*>{};
      }
      std::vector<const IManifold<T, 2>*> GetConnectedHighers(const IManifold<T, 2>& manifold) const override { return std::vector<const IManifold<T, 2>*>{}; }
   private:
      const Sphere2AsManifold1<T> m_sphereManifold;
   };
}

template<typename T>
Ball2AsRegion<T>::Ball2AsRegion(Ball<T, 2> ball) :
   m_ball(std::move(ball)),
   m_sphereManifolds(std::make_unique<BallManifolds<T>>(Sphere2AsManifold1<T>(Sphere<T, 2>(m_ball.getCenter(), m_ball.getRadius()))))
{
}

template<typename T>
BoundingBox<T, 2> Ball2AsRegion<T>::getBoundingBox() const
{
   return m_ball.getBoundingBox();
}

template<typename T>
bool Ball2AsRegion<T>::Contains(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   return m_ball.Contains(point, predicate);
}

template<typename T>
bool Ball2AsRegion<T>::CouldIntersectWith(const BoundingBox<T, GeomDim>& bb, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   return m_ball.CouldIntersectWith(bb, predicate);
}

template<typename T>
const IRegionManifolds<T, 2>& Ball2AsRegion<T>::getManifolds() const
{
   return *m_sphereManifolds;
}