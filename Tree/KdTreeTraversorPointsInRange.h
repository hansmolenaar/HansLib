#pragma once

#include "IKdTreeTraversor.h"
#include "BoundingBox.h"

#include <vector>

template<typename T, size_t N>
class KdTree;


template<typename T, size_t N>
class KdTreeTraversorPointsInRange : public IKdTreeTraversor<T, N>
{
public:
   KdTreeTraversorPointsInRange(BoundingBox<T, N> bb) : m_bb(bb) {}
   void HandleLeaf(KdTreePosition position, const  Point<T, N>&) override;
   KdTreeOverlap DeterminOverlap(const Point<T, N>&, const Point<T, N>&) const override;
   const std::vector< KdTreePosition>& GetFound() const { return  m_found; }

private:
   std::vector<KdTreePosition> m_found;
   BoundingBox<T, N> m_bb;
};

template<typename T, size_t N>
void KdTreeTraversorPointsInRange<T, N>::HandleLeaf(KdTreePosition position, const  Point<T, N>& point)
{
   if (m_bb.contains(point))
   {
      m_found.push_back(position);
   }
}

template<typename T, size_t N>
KdTreeOverlap KdTreeTraversorPointsInRange<T, N>::DeterminOverlap(const Point<T, N>& lwrBound, const Point<T, N>& uprBound) const
{
   auto result = KdTreeOverlap::Contains;
   for (int d = 0; d < N; ++d)
   {
      if (m_bb.getLower(d) > uprBound.data()[d])
      {
         return KdTreeOverlap::NoOverlap;
      }
      if (m_bb.getUpper(d) < lwrBound.data()[d])
      {
         return KdTreeOverlap::NoOverlap;
      }
      if (m_bb.getLower(d) > lwrBound.data()[d])
      {
         result = KdTreeOverlap::Overlap;
      }
      if (m_bb.getUpper(d) < uprBound.data()[d])
      {
         result = KdTreeOverlap::Overlap;
      }
   }
   return result;
}
