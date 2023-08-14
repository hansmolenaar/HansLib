#include "Point.h"
#include <boost/functional/hash.hpp>

namespace
{

   template<typename T, int N>
   size_t Hash(const Point<T, N>& point)
   {
      size_t result = 0;
      for (int n = 0; n < N; ++n)
      {
         boost::hash_combine(result, std::hash<T>{}(point[n]));
      }
      return result;
   }
}

size_t std::hash<IntPoint1>::operator()(const IntPoint1& point) const noexcept
{
   return Hash(point);
}


size_t std::hash<IntPoint2>::operator()(const IntPoint2& point) const noexcept
{
   return Hash(point);
}

size_t std::hash<IntPoint3>::operator()(const IntPoint3& point) const noexcept
{
   return Hash(point);
}

size_t std::hash<RatPoint1>::operator()(const RatPoint1& point) const noexcept
{
   return Hash(point);
}

size_t std::hash<RatPoint2>::operator()(const RatPoint2& point) const noexcept
{
   return Hash(point);
}

size_t std::hash<RatPoint3>::operator()(const RatPoint3& point) const noexcept
{
   Point1 p{1.0};
   return Hash(p);
   return Hash(point);
}