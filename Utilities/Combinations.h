#pragma once

#include <numeric>
#include <span>
#include <vector>

#include "MyAssert.h"

namespace Utilities
{

size_t countCombinations(size_t, size_t);
bool next_combination(std::vector<int> &a, int n);

template <typename T> class Combinations
{
 public:
   Combinations(std::span<const T> allValues, size_t r)
       : m_N(allValues.size()), m_r(r), m_positions(m_r), m_original(allValues),
         m_values(allValues.begin(), allValues.begin() + r)
   {
      MyAssert(m_r <= m_N);
      std::iota(m_positions.begin(), m_positions.end(), 1);
   }

   const std::vector<T> &operator()() const
   {
      return m_values;
   }

   bool next()
   {
      bool result = next_combination(m_positions, m_N);
      for (size_t n = 0; n < m_r; ++n)
      {
         m_values[n] = m_original[m_positions[n] - 1];
      }
      return result;
   }

 private:
   size_t m_N;
   size_t m_r;
   std::vector<int> m_positions;
   std::span<const T> m_original;
   std::vector<T> m_values;
};
} // namespace Utilities
