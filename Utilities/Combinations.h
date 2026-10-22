#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <span>
#include <vector>

#include "MyAssert.h"

namespace Utilities
{

size_t countCombinations(size_t, size_t);
bool next_combination(std::vector<int> &a, int n);

template <class iterator_type, class output_iterator> class combination_generator
{
   iterator_type first, last;
   std::vector<bool> use;
   output_iterator o_first;
   output_iterator o_last;
   size_t r;

 public:
   combination_generator(iterator_type first_, iterator_type last_, output_iterator o_first_, output_iterator o_last_)
       : first(first_), last(last_), o_first(o_first_), o_last(o_last_), r(std::distance(o_first, o_last))
   {
      use.resize(std::distance(first, last), false);
      MyAssert(r <= use.size());
      std::fill(use.end() - r, use.end(), true);
      setOutput();
   }
   bool operator()()
   {
      const bool retval = std::next_permutation(use.begin(), use.end());
      setOutput();
      return retval;
   }

 private:
   void setOutput()
   {
      iterator_type c = first;
      output_iterator result = o_first;
      for (unsigned i = 0; i < use.size(); ++i, ++c)
      {
         if (use[i])
            *result++ = *c;
      }
   }
};

template <class iterator_type, class output_iterator>
combination_generator<iterator_type, output_iterator> make_combination_generator(iterator_type first,
                                                                                 iterator_type last,
                                                                                 output_iterator d_first,
                                                                                 output_iterator d_last)
{
   return combination_generator(first, last, d_first, d_last);
}

template <typename T> class CombinationS
{
 public:
   CombinationS(std::span<const T> allValues, size_t r)
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
