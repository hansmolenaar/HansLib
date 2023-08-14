#pragma once

#include "IPointCollection.h"
#include "IGeometryPredicate.h"

#include <unordered_map>



#if false
template<typename T, int N>
class UniqueHashedPointCollection : public IUniquePointCollection<T, N>
{
public:
   // size_t H(const Point<T, N>&);
   template<typename H>
   explicit UniqueHashedPointCollection(H& hasher);
   int add(const Point<T, N>&);
   virtual std::tuple<bool, int>  tryGetClosePoint(const Point<T, N>&) const = 0;
   virtual const IGeometryPredicate<T, N>& getGeometryPredicate() const = 0;
private:
   H& hasher m_hasher;
   std::unordered_map<Point<T, N>, int> m_toIndex;
   std::vector< Point<T, N> m_points;
};
#endif