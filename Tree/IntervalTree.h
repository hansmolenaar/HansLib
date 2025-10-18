#pragma once

#include "IntervalTreeAction.h"
#include "IntervalTreeIndexFactory.h"
#include "Logger.h"
#include "StdHash.h"

#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace IntervalTree
{

   template<size_t N>
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

      template<typename A>
      void foreachNode(A& action) const;

      bool contains(const Index<N>::Key& key) const;

      const Index<N>& getExistingSelfOrAncestor(Index<N>::Key key) const;
      std::optional<const Index<N>*> get(const Index<N>::Key& key) const;
      bool isLeaf(const Index<N>& index) const;

      size_t size() const;

      friend std::ostream& operator<<(std::ostream& os, const IndexTree& tree)
      {
         static const std::string sep = "  ";
         os << "IndexTree N=" << N;
         os << sep << "SIZE=" << tree.size();
         os << sep << "NUMLEAVES=" << tree.m_leaves.size();

         ActionMaxLevel<N> actionMaxLevel;
         tree.foreachLeaf(actionMaxLevel);
         os << sep << "MAXLEVEL=" << actionMaxLevel.MaxLevel;
         return os;
      }

   private:
      std::vector<const Index<N>*> getLeavesInFixedOrder() const;
      std::vector<const Index<N>*> getNodesInFixedOrder() const;

      IndexFactory<N> m_factory;
      std::unordered_map <const Index<N>*, std::array< const Index<N>*, IntervalTree::NumKids<N>>> m_tree;
      std::unordered_set<const Index<N>*> m_leaves;
      const Index<N>* m_root;
   };

   template<size_t N>
   IndexTree<N>::IndexTree() : m_root(m_factory.getRoot())
   {
      m_leaves.insert(m_root);
   }

   template<size_t N>
   std::vector<const Index<N>*> IndexTree<N>::getNodesInFixedOrder() const
   {
      std::vector<const Index<N>*> result(m_leaves.begin(), m_leaves.end());
      str::transform(m_tree, std::back_inserter(result), [](const auto& itr) {return itr.first; });
      std::sort(result.begin(), result.end(), IntervalTree::ComparePointer<N>());
      return result;
   }

   template<size_t N>
   std::vector<const Index<N>*> IndexTree<N>::getLeavesInFixedOrder() const
   {
      std::vector<const Index<N>*> result(m_leaves.begin(), m_leaves.end());
      std::sort(result.begin(), result.end(), IntervalTree::ComparePointer<N>());
      return result;
   }

   template<size_t N>
   const Index<N>& IndexTree<N>::getRoot() const
   {
      return *m_root;
   }

   template<size_t N>
   size_t IndexTree<N>::size() const
   {
      return m_tree.size() + m_leaves.size();
   }

   template<size_t N>
   template<typename A>
   void IndexTree<N>::foreachLeaf(A& action) const
   {
      for (const auto* leaf : getLeavesInFixedOrder())
      {
         action(*leaf);
      }
   }

   template<size_t N>
   template<typename A>
   void IndexTree<N>::foreachNode(A& action) const
   {
      for (const auto* node : getNodesInFixedOrder())
      {
         action(*node);
      }
   }

   template<size_t N>
   template<typename P>
   int IndexTree<N>::refineLeaves(P& predicate)
   {
      std::vector<const Index<N>*> toRefine;
      for (const auto* leaf : getLeavesInFixedOrder())
      {
         const bool doRefine = predicate(*leaf);
         if (doRefine)
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

   template<size_t N>
   template<typename P>
   void IndexTree<N>::refineUntilReady(P& predicate)
   {
      while (refineLeaves(predicate) > 0)
      {
      }
   }

   template<size_t N>
   std::optional<const Index<N>*> IndexTree<N>::get(const Index<N>::Key& key) const
   {
      const auto  ptr = m_factory.get(key);
      if (!ptr) return {};

      if (m_leaves.contains(*ptr)) return  ptr;
      if (m_tree.contains(*ptr)) return ptr;
      return {};
   }

   template<size_t N>
   bool IndexTree<N>::contains(const Index<N>::Key& key) const
   {
      return get(key).has_value();
   }

   template<size_t N>
   bool IndexTree<N>::isLeaf(const Index<N>& index) const
   {
      if (!contains(index.getKey()))
      {
         std::ostringstream os;
         os << "IndexTree<N>::isLeaf() unknown key specified: " << index;
         throw MyException(os.str());
      }
      return m_leaves.contains(&index);
   }

   template<size_t N>
   const Index<N>& IndexTree<N>::getExistingSelfOrAncestor(Index<N>::Key key) const
   {
      while (true)
      {
         const auto index = get(key);
         if (index) return *(index.value());
         key = Index<N>::GetParent(key);
      }
   }

}
