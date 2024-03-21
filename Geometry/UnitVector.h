#pragma once

#include "Defines.h"
#include "Point.h"

#include <numeric>
#include <limits>
#include <cmath>
#include <span>
#include <optional>

template<typename T, int N>
class UnitVector
{
public:
   static std::optional<UnitVector<T, N>> Create(std::span<const T>);
   static std::optional<UnitVector<T, N>> Create(const Point<T, N>&);
   T operator[](int) const;
   const std::array<T, N>& data() const { return m_vector; }

   T innerProduct(const Point<T, N>&) const;
private:
   UnitVector(std::array<T, N>&&);
   Point<T, N> m_vector;
};

template<typename T, int N>
UnitVector<T, N>::UnitVector(std::array<T, N>&& unitVector) :
   m_vector(unitVector)
{
}

template<typename T, int N>
T UnitVector<T, N>::operator[](int d) const
{
   return m_vector.at(d);
}

template<typename T, int N>
std::optional<UnitVector<T, N>>  UnitVector<T, N>::Create(const Point<T, N>& point)
{
   return Create(std::span<const T>(point.begin(), point.end()));
}

template<typename T, int N>
std::optional<UnitVector<T, N>>  UnitVector<T, N>::Create(std::span<const T> cors)
{
   Utilities::MyAssert(cors.size() == N, "UnitVector<N>::Create span dimension incorrect");
   const T norm2 = std::accumulate(cors.begin(), cors.end(), 0.0, [](T v0, T v1) { return v0 + v1 * v1; });
   if (norm2 < std::numeric_limits<T>::min()) return std::nullopt;
   const T norm = std::sqrt(norm2);
   std::array<T, N> values;
   for (int n = 0; n < N; ++n)
   {
      values[n] = cors[n] / norm;
   }
   return UnitVector<T, N>(std::move(values));
}

template<typename T, int N>
Point<T, N> operator*(const UnitVector<T, N>& uv, T factor)
{
   std::array<T, N> result;
   str::transform(uv.data(), result.begin(), [factor](auto value) {return factor * value; });
   return Point<T, N>{ result };
}


template<typename T, int N>
Point<T, N> operator*(T factor, const UnitVector<T, N>& uv)
{
   return uv * factor;
}

template<typename T, int N>
T UnitVector<T, N>::innerProduct(const Point<T, N>& p) const
{
   return std::inner_product(m_vector.begin(), m_vector.end(), p.begin(), 0.0);
}

