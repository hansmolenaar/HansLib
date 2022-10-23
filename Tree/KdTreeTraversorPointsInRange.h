#pragma once

#include "IKdTreeTraversor.h"
#include "BoundingBox/BoundingBox.h"

#include <vector>

template<typename T, int N>
class KdTree;


template<typename T, int N>
class KdTreeTraversorPointsInRange : public IKdTreeTraversor<T,N>
{
public:
   KdTreeTraversorPointsInRange(const KdTree<T, N>& tree, BoundingBox<T, N> bb) : m_bb(bb), m_tree(tree) {}
   void HandleLeaf(KdTreePosition position) override;
   KdTreeOverlap DeterminOverlap(const Point<T, N>&, const Point<T, N>&) const override;

private:
   std::vector<KdTreePosition> m_found;
   BoundingBox<T, N> m_bb;
   const KdTree<T, N>& m_tree;
};

template<typename T, int N>
void KdTreeTraversorPointsInRange<T, N>::HandleLeaf(KdTreePosition position)
{
   if (m_bb.contains(m_tree.GetPoint(position)))
   {
      m_found.push_back(position);
   }
}

template<typename T, int N>
KdTreeOverlap KdTreeTraversorPointsInRange<T, N>::DeterminOverlap(const Point<T, N>& lwrBound, const Point<T, N>& uprBound) const
{
   auto result = KdTreeOverlap::Contains;
   for (int d = 0; d < N; ++d)
   {
      if (m_bb.getLower(d) > uprBound[d])
      {
         return KdTreeOverlap::NoOverlap;
      }
      if (m_bb.getUpper(d) < lwrBound[d])
      {
         return KdTreeOverlap::NoOverlap;
      }
      if (m_bb.getLower(d) > lwrBound[d])
      {
         result = KdTreeOverlap::Overlap;
      }
      if (m_bb.getUpper(d) < uprBound[d])
      {
         result = KdTreeOverlap::Overlap;
      }
   }
   return result;
}
