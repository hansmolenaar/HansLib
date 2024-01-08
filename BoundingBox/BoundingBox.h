#pragma once

#include "Defines.h"
#include "Interval.h"
#include "MyAssert.h"
#include "Point.h"

#include <span>
#include <optional>

template <typename T, size_t... Is, typename... Args>
std::array<T, sizeof...(Is)> MakeArrayHelper(
   std::index_sequence<Is...>, Args&&... args) {
   return { (static_cast<void>(Is), T{std::forward<Args>(args)...}) ... };
}

template <typename T, size_t N, typename... Args>
std::array<T, N> MakeArray(Args&&... args) {
   return MakeArrayHelper<T>(std::make_index_sequence<N>{},
      std::forward<Args>(args)...);
}

template<typename T, int N>
class BoundingBox
{
public:
   static BoundingBox Create(const std::span<const T>&);

   template<typename Container>
   static BoundingBox CreateFromList(const Container& container);

   static BoundingBox CreateFrom2Points(const Point<T, N>& point1, const Point<T, N>& point2);

   template<typename TScale>
   BoundingBox<T, N> scaleFrom01(const BoundingBox<TScale, N>& scale) const;

   template<typename TScale>
   Point<T, N> scaleFromPoint01(const Point<TScale, N>& scale) const;

   template<typename Container, typename F>
   static BoundingBox CreateFromListTransformed(const Container& container, F fun);

   const Interval<T>& getInterval(int n) const { return m_intervals.at(n); }
   std::span< const Interval<T>> getIntervals() const { return m_intervals; }

   static std::optional<BoundingBox<T, N>> TryGetOverlap(const BoundingBox<T, N>& bb1, const BoundingBox<T, N>& bb2);

   Point<T, N> getLower() const;
   Point<T, N> getUpper() const;

   T getLower(int) const;
   T getUpper(int) const;

   void Add(const std::span<const T>&);

   bool contains(const Point<T, N>&) const;

   Point<T, N> getCenter() const;
   T getLengthDiagonalSquared() const;

   auto operator<=>(const BoundingBox<T, N>&) const = default;

private:
   explicit BoundingBox(const std::span<const T>&);

   std::array<Interval<T>, N> m_intervals;
};


template<typename T, int N>
BoundingBox<T, N> BoundingBox<T, N>::Create(const std::span<const T>& values)

{
   return BoundingBox<T, N>(values);
};

template<typename T, int N>
BoundingBox<T, N>::BoundingBox(const std::span<const T>& values) :
   m_intervals(MakeArray<Interval<T>, N>(Interval<T>(T{})))
{
   Utilities::MyAssert(values.size() == N);
   for (int n = 0; n < N; ++n)
   {
      m_intervals.at(n) = Interval<T>(values[n]);
   }
};


template<typename T, int N>
void BoundingBox<T, N>::Add(const std::span<const T>& values)

{
   for (auto n = 0; n < N; ++n)
   {
      m_intervals.at(n).add(values[n]);
   }
};

template<typename T, int N >
template<typename Container>
BoundingBox<T, N> BoundingBox<T, N>::CreateFromList(const Container& container)
{
   auto itr = std::begin(container);
   auto result = BoundingBox<T, N>::Create(*itr);
   ++itr;
   for (; itr != std::end(container); ++itr)
   {
      result.Add(*itr);
   }
   return result;
}

template<typename T, int N>
BoundingBox<T, N> BoundingBox<T, N>::CreateFrom2Points(const Point<T, N>& point1, const Point<T, N>& point2)
{
   BoundingBox<T, N> result = BoundingBox<T, N>::Create(point1);
   result.Add(point2);
   return result;
}


template<typename T, int N >
template<typename Container, typename F>
BoundingBox<T, N> BoundingBox<T, N>::CreateFromListTransformed(const Container& container, F fun)
{
   auto itr = std::begin(container);
   std::array<int, 1> tmp = fun(*itr);
   BoundingBox<T, N> result = BoundingBox<T, N>::Create(fun(*itr));
   ++itr;
   for (; itr != std::end(container); ++itr)
   {
      result.Add(fun(*itr));
   }
   return result;
}


template<typename T, int N >
template<typename TScale>
BoundingBox<T, N> BoundingBox<T, N>::scaleFrom01(const BoundingBox<TScale, N>& scale) const
{
   const Point<T, N> pointLwr = scaleFromPoint01(scale.getLower());
   const Point<T, N> pointUpr = scaleFromPoint01(scale.getUpper());
   return CreateFrom2Points(pointLwr, pointUpr);
}


template<typename T, int N >
template<typename TScale>
Point<T, N> BoundingBox<T, N>::scaleFromPoint01(const Point<TScale, N>& scale) const
{
   Point<T, N> point;
   for (int n = 0; n < N; ++n)
   {
      const T lwr = m_intervals[n].getLower();
      const T upr = m_intervals[n].getUpper();
      point[n] = lwr + scale[n] * (upr - lwr);
   }
   return point;
}

template<typename T, int N >
Point<T, N> BoundingBox<T, N>::getLower() const
{
   std::array<T, N> values;
   str::transform(m_intervals, values.data(), [](const Interval<T>& intv) {return intv.getLower(); });
   return Point<T, N>(values);
}

template<typename T, int N >
Point<T, N> BoundingBox<T, N>::getUpper() const
{
   std::array<T, N> values;
   str::transform(m_intervals, values.data(), [](const Interval<T>& intv) {return intv.getUpper(); });
   return Point<T, N>(values);
}


template<typename T, int N >
T BoundingBox<T, N>::getLower(int d) const
{
   return m_intervals.at(d).getLower();
}

template<typename T, int N >
T BoundingBox<T, N>::getUpper(int d) const
{
   return m_intervals.at(d).getUpper();
}

template<typename T, int N >
bool BoundingBox<T, N>::contains(const Point<T, N>& point) const
{
   for (int d = 0; d < N; ++d)
   {
      if (!m_intervals.at(d).contains(point.at(d))) return false;
   }
   return true;
}

template<typename T, int N >
std::optional<BoundingBox<T, N>> BoundingBox<T, N>::TryGetOverlap(const BoundingBox<T, N>& bb1, const BoundingBox<T, N>& bb2)
{
   std::array<T, N> lwr;
   std::array<T, N> upr;
   for (int n = 0; n < N; ++n)
   {
      if (!Interval<T>::TryIntersect(bb1.getInterval(n), bb2.getInterval(n), lwr[n], upr[n]))
      {
         return {};
      }
   }
   return CreateFromList(std::array < std::array<T, N>, 2> {lwr, upr});
}

template<typename T, int N >
Point<T, N> BoundingBox<T, N>::getCenter() const
{
   Point<T, N> result;
   str::transform(getIntervals(), result.begin(), [](const Interval<T>& intv) {return (intv.getLower() + intv.getUpper()) / 2; });
   return result;
}

template<typename T, int N >
T BoundingBox<T, N>::getLengthDiagonalSquared() const
{
   const auto dif = getUpper() - getLower();
   return  PointUtils::GetNormSquared(dif);
}