#pragma once

#include "IntervalTreeIndex.h"
#include <map>
#include <unordered_set>

namespace IntervalTree
{
   template<int N>
   class IndexTree
   {
   public:
      IndexTree();
      const Index<N>& getRoot() const;

      template<typename P>
      int refineLeaves(P& predicate);

      template<typename A>
      void foreachLeaf(A& action) const;
   private:
      IndexFactory<N> m_factory;
      std::unordered_multimap <const Index<N>*, const Index<N>* > m_tree;
      std::unordered_set<const Index<N>*> m_leaves;
      const Index<N>* m_root;
   };

   template<int N>
   IndexTree<N>::IndexTree() : m_root(m_factory.getRoot())
   {
      m_leaves.insert(m_root);
   }

   template<int N>
   const Index<N>& IndexTree<N>::getRoot() const
   {
      return *m_root;
   }

   template<int N>
   template<typename A>
   void IndexTree<N>::foreachLeaf(A& action) const
   {
      for (const auto* leaf : m_leaves)
      {
         action(*leaf);
      }
   }
}