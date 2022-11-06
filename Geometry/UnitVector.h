#pragma once

#include "Utilities/Defines.h"
#include "Point.h"

#include <numeric>
#include <limits>
#include <cmath>

template<int N>
class UnitVector
{
public:
   static std::unique_ptr<UnitVector<N>> Create(std::span<const double>);
   double operator[](int) const;
   Point<double, N> operator()(double) const;
   double innerProduct(const Point<double, N>&) const;
private:
   UnitVector(std::array<double, N>&&);
   Point<double, N> m_vector;
};

template<int N>
UnitVector<N>::UnitVector(std::array<double, N>&& unitVector) :
   m_vector(unitVector)
{
}

template<int N>
double UnitVector<N>::operator[](int d) const
{
   return m_vector[d];
}

template<int N>
std::unique_ptr<UnitVector<N>>  UnitVector<N>::Create(std::span<const double> cors)
{
   std::unique_ptr<UnitVector<N>> result;
   const double norm2 = std::accumulate(cors.begin(), cors.end(), 0.0, [](double v0, double v1) { return v0 + v1 * v1; });
   if (norm2 < std::numeric_limits<double>::min()) return result;
   const double norm = std::sqrt(norm2);
   std::array<double, N> values;
   for (int n = 0; n < N; ++n)
   {
      values[n] = cors[n] / norm;
   }
   result.reset(new UnitVector<N>(std::move(values)));
   return result;
 }

template<int N>
Point<double, N> UnitVector<N>::operator()(double factor) const
{

}

template<int N>
double UnitVector<N>::innerProduct(const Point<double, N>& p) const
{
   return std::inner_product(m_vector.begin(), m_vector.end(), p.begin(), 0.0);
}

