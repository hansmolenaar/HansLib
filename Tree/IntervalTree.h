#pragma once

#include "IntervalTreeIndex.h"
#include <map>

namespace IntervalTree
{
   template<int N>
   class IndexTree
   {
   public:
      IndexTree();
      const Index<N>* getRoot();
      void refine(const Index<N>& toRefine);

      template<typename F>
      void foreachKid(F fun) const;

      template<typename F>
      void foreachLeaf(F fun) const;
   private:
      IndexFactory<N> m_factory;
      std::multimap < Index<N>::Key, Index<N>::Key >
   };
}