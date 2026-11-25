#pragma once

#include <bitset>
#include <span>
#include <stdexcept>

namespace Utilities
{

template <std::size_t N> struct Bitset
{
   std::bitset<N> m_bitset;
   bool operator()(size_t pos) const
   {
      return m_bitset.test(pos);
   }

   size_t first() const
   {
      for (size_t n = 0; n < N; ++n)
      {
         if (m_bitset.test(n))
         {
            return n;
         }
      }
      throw std::runtime_error("Bitset: not bits set");
   }

   static std::bitset<N> create(std::span<const unsigned short> positions)
   {
      std::bitset<N> result;
      for (auto p : positions)
      {
         if (positions.size() >= N)
         {
            throw std::runtime_error("Bitset: incorrect position");
         }
         result.set(p);
      }
      return result;
   }
};

} // namespace Utilities
