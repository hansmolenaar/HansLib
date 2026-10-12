#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "MyAssert.h"

namespace Utilities
{

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

} // namespace Utilities
