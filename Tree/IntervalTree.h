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
      const Index<N>* getRoot() const;
      void refine(const Index<N>& toRefine);

      template<typename F>
      void foreachKid(F fun) const;

      template<typename F>
      void foreachLeaf(F fun) const;
   private:
      IndexFactory<N> m_factory;
      std::unordered_multimap <typename Index<N>::Key, typename Index<N>::Key > m_tree;
      const Index<N>* m_root;
   };

   template<int N>
   IndexTree<N>::IndexTree() : m_root(m_factory.getRoot())
   {
   }

   template<int N>
   const Index<N>* IndexTree<N>::getRoot() const
   {
      return m_root;
   }

}