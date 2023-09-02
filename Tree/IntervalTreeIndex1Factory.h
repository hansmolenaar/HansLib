#pragma once

#include "IntervalTreeIndex1.h"
#include <unordered_map>

namespace IntervalTree
{
   class Index1Factory
   {
   public:
      Index1Factory();
      const Index1* operator()(Index1::Key key) const;
      Index1::Key add(const Index1& index1);
      Index1::Key add(const Interval<Rational>& interval);
      const Index1* getRoot() const;
      std::array<const Index1*, 2> refine(const Index1& toRefine);
      const Index1* getParent(Index1::Key key) const;
      const Index1* getParent(const Index1& indx1) const;

   private:
      std::unordered_map<Index1::Key, Index1> m_cache;
      std::unordered_map<Index1::Key, Index1::Key> m_toParent;
   };
}
