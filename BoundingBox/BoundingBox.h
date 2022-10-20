#pragma once

#include <array>
#include <span>
#include "../Interval/Interval.h"
#include "../HLUtils/MessageHandler.h"

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
   explicit BoundingBox(const std::span<const T>&);
   const Interval<T>& getInterval(int n) const { return m_intervals.at(n); }
private:
   std::array<Interval<T>, N> m_intervals;
};


template<typename T, int N>
BoundingBox<T, N>::BoundingBox(const std::span<const T>& values) :
   m_intervals(MakeArray<Interval<T>, N>(Interval<T>(T{})))
{
   MessageHandler::Assert(values.size() == N);
   for (int n = 0; n < N; ++n)
   {
      m_intervals.at(n) = Interval<T>(values[n]);
   }
};
