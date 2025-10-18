#pragma once

#include "Defines.h"
#include "Point.h"

#include <cmath>
#include <limits>
#include <numeric>
#include <optional>
#include <span>

template<typename T, size_t N>
class UnitVector
{
public:
   UnitVector() = default;
   bool isValid() const { return static_cast<bool>(m_vector); }
   static UnitVector<T, N> Create(std::span<const T>);
   static UnitVector<T, N> Create(const Point<T, N>&);
   static UnitVector<T, N> Create(const Point<T, N>& from, const Point<T, N>& to);
   static UnitVector<T, N> getNormalTo(Point<T, N> v);

   T operator[](int n) const;
   const std::array<T, N>& data() const { return *m_vector; }

   T innerProduct(const Point<T, N>&) const;

   // inner product
   friend T operator*(const UnitVector<T, N>& lhs, const UnitVector<T, N>& rhs) { return PointUtils::innerProduct(lhs.data(), rhs.data()); };

private:
   UnitVector(std::array<T, N>&&);
   std::optional<Point<T, N>> m_vector;
};

using UnitVector2 = UnitVector<double, GeomDim2>;
using UnitVector3 = UnitVector<double, GeomDim3>;

template<typename T, size_t N>
UnitVector<T, N>::UnitVector(std::array<T, N>&& unitVector) :
   m_vector(unitVector)
{
}

template<typename T, size_t N>
T UnitVector<T, N>::operator[](int d) const
{
   return m_vector->at(d);
}

template<typename T, size_t N>
UnitVector<T, N> UnitVector<T, N>::Create(const Point<T, N>& from, const Point<T, N>& to)
{
   return Create(to - from);
}

template<typename T, size_t N>
UnitVector<T, N>  UnitVector<T, N>::Create(const Point<T, N>& point)
{
   return Create(std::span<const T>(point.begin(), point.end()));
}

template<typename T, size_t N>
UnitVector<T, N>  UnitVector<T, N>::Create(std::span<const T> cors)
{
   Utilities::MyAssert(cors.size() == N, "UnitVector<N>::Create span dimension incorrect");
   T norm2 = 0;
   norm2 = std::accumulate(cors.begin(), cors.end(), norm2, [](T v0, T v1) { return v0 + v1 * v1; });
   if (norm2 < std::numeric_limits<T>::min()) return {};
   const T norm = std::sqrt(norm2);
   std::array<T, N> values;
   for (int n = 0; n < N; ++n)
   {
      values[n] = cors[n] / norm;
   }
   return UnitVector<T, N>(std::move(values));
}

template<typename T, size_t N>
Point<T, N> operator*(const UnitVector<T, N>& uv, T factor)
{
   std::array<T, N> result;
   str::transform(uv.data(), result.begin(), [factor](auto value) {return factor * value; });
   return Point<T, N>{ result };
}


template<typename T, size_t N>
Point<T, N> operator*(T factor, const UnitVector<T, N>& uv)
{
   return uv * factor;
}

template<typename T, size_t N>
T UnitVector<T, N>::innerProduct(const Point<T, N>& p) const
{
   return std::inner_product(m_vector->begin(), m_vector->end(), p.begin(), 0.0);
}

template<typename T, size_t N>
UnitVector<T, N> UnitVector<T, N>::getNormalTo(Point<T, N> v)
{
   static_assert(N == 2);
   std::swap(v[0], v[1]);
   v[0] = -v[0];
   return UnitVector<T, GeomDim2>::Create(v);
}
