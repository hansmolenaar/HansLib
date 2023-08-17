#pragma once

#include "Rational.h"
#include "IntervalTree.h"

namespace IntervalTree
{

   template<int N>
   struct ActionCount
   {
      void operator()(const Index<N>&) { ++m_count; };
      int operator()() const { return m_count; };
      void reset() { m_count = 0; }
   private:
      int m_count = 0;
   };

}
