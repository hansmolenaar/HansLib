
#include "IManifold0.h"

namespace Geometry
{

   template<typename T, int N>
   class Manifold0 : public IManifold0<T,N>
   {
   public:
      explicit Manifold0<T, N>(const Point<T, N>& point);

       Point<T, N> GetPoint() const override;

      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Corner; };
   private:
      Point<T, N> m_point;
   };

   template<typename T, int N>
   Manifold0<T, N>::Manifold0(const Point<T, N>& point) : m_point(point)
   {}

   template<typename T, int N>
   Point<T, N> Manifold0<T, N>::GetPoint() const
   {
      return m_point;
   }
}