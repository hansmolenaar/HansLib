#pragma once

#include "IGeometryPredicate.h"
#include "MyAssert.h"
#include "Point.h"

template <typename T, size_t N> class PointClose : public IGeometryPredicate<T, N>
{
  public:
    bool operator()(const Point<T, N> &, const Point<T, N> &) const;
    bool samePoints(Point<T, N> p0, Point<T, N> p1) const override;
    T getSmallLengthInDirection(int n) const override;
    T getSmallNormSquared() const override;

  private:
    static constexpr double eps = 1.0e-7;
    static constexpr T m_smallInDirection = static_cast<T>(eps);
};

template <typename T, size_t N> bool PointClose<T, N>::operator()(const Point<T, N> &p0, const Point<T, N> &p1) const
{
    const auto dif = p0 - p1;
    return str::all_of(dif, [this](auto d) { return std::abs(d) < eps; });
}

template <typename T, size_t N> bool PointClose<T, N>::samePoints(Point<T, N> p0, Point<T, N> p1) const
{
    return (*this)(p0, p1);
}

template <typename T, size_t N> T PointClose<T, N>::getSmallLengthInDirection(int n) const
{
    Utilities::MyAssert(n >= 0);
    Utilities::MyAssert(n < N);
    return m_smallInDirection;
}

template <typename T, size_t N> T PointClose<T, N>::getSmallNormSquared() const
{
    constexpr T result = N * m_smallInDirection;
    return result;
}
