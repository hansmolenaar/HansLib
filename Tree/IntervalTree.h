#pragma once

#include "IntervalTreeIndexFactory.h"
#include "IntervalTreeAction.h"

#include <unordered_map>
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

      template<typename P>
      void refineUntilReady(P& predicate);

      template<typename A>
      void foreachLeaf(A& action) const;

      bool contains(typename const Index<N>::Key& key) const;

      const Index<N>& getExistingSelfOrAncestor(typename  Index<N>::Key key) const;
      std::tuple<bool, const Index<N>&> get(typename const Index<N>::Key& key) const;

      size_t size() const;

   private:
      IndexFactory<N> m_factory;
      std::unordered_map < const Index<N>*, std::array< const Index<N>*, IntervalTree::NumKids<N>>> m_tree;
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
   size_t IndexTree<N>::size() const
   {
      return m_tree.size() + m_leaves.size();
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

   template<int N>
   template<typename P>
   int IndexTree<N>::refineLeaves(P& predicate)
   {
      std::vector<const Index<N>*> toRefine;
      for (const auto* leaf : m_leaves)
      {
         if (predicate(*leaf))
         {
            toRefine.emplace_back(leaf);
         }
      }

      for (const auto* leaf : toRefine)
      {
         const auto kids = m_factory.refine(*leaf);
         m_tree.emplace(leaf, kids);
         for (const auto* kid : kids)
         {
            m_leaves.insert(kid);
         }
         m_leaves.erase(leaf);
      }

      return static_cast<int>(toRefine.size());
   }

   template<int N>
   template<typename P>
   void IndexTree<N>::refineUntilReady(P& predicate)
   {
      while (refineLeaves(predicate) > 0)
      {
      }
   }

   template<int N>
   std::tuple<bool, const Index<N>&> IndexTree<N>::get(typename const Index<N>::Key& key) const
   {
      const auto [succes, ptr] = m_factory.get(key);
      if (!succes) return { false, getRoot() };

      if (m_leaves.contains(ptr)) return { true, *ptr };
      if (m_tree.contains(ptr)) return { true, *ptr };
      return { false, getRoot() };
   }

   template<int N>
   bool IndexTree<N>::contains(typename const Index<N>::Key& key) const
   {
      return std::get<0>(get(key));
   }

   template<int N>
   const Index<N>& IndexTree<N>::getExistingSelfOrAncestor(typename  Index<N>::Key key) const
   {
      while (true)
      {
         const auto [found, index] = get(key);
         if (found) return index;
         key = Index<N>::GetParent(key);
      }
   }

}
