#pragma once

#include "IntervalTreeIndexFactory.h"
#include "IntervalTreeAction.h"
#include "VtkData.h"

#include <map>
#include <unordered_set>

namespace IntervalTree
{
   struct Statistics
   {
      int Size= -1;
      std::vector<int> NumberOfLeavesPerLevel;
      std::string toString() const;
      auto operator<=>(const Statistics&) const = default;
   };

   template<int N>
   class IndexTree
   {
   public:
      IndexTree();
      const Index<N>& getRoot() const;
      Statistics getStatistics() const;
      std::unique_ptr<Vtk::VtkData> getVtkData() const;

      template<typename P>
      int refineLeaves(P& predicate);

      template<typename P>
      void refineUntilReady(P& predicate);

      template<typename A>
      void foreachLeaf(A& action) const;
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
   Statistics IndexTree<N>::getStatistics() const
   {
      Statistics result;

      std::unordered_set<const Index<N>*> keys;
      for (const auto& itr : m_tree)
      {
         keys.insert(itr.first);
      }

      std::map<int, int> count;
      for (auto itr : m_leaves)
      {
         count[itr->getLevel()] += 1;
      }
      result.Size = static_cast<int>(keys.size() + m_leaves.size());

      result.NumberOfLeavesPerLevel = std::vector<int>(count.rbegin()->first + 1, 0);
      for (const auto& itr : count)
      {
         result.NumberOfLeavesPerLevel.at(itr.first) = itr.second;
      }

      return result;
   }

   template<>
   std::unique_ptr< Vtk::VtkData> IndexTree<1>::getVtkData() const;

   template<>
   std::unique_ptr< Vtk::VtkData> IndexTree<2>::getVtkData() const;

   template<>
   std::unique_ptr< Vtk::VtkData> IndexTree<3>::getVtkData() const;
}
