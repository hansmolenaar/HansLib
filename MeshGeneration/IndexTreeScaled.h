#pragma once

#include "Interval.h"
#include "BoundingBox.h"
#include "IntervalTree.h"
#include "IntervalTreeIndex.h"
#include "Defines.h"

namespace MeshGeneration
{
   template<typename T, size_t N>
   class IndexTreeScaled
   {
   public:
      IndexTreeScaled(std::unique_ptr< IntervalTree::IndexTree<N>> tree, BoundingBox<T, N> bb);
      Point<T, N> scaleCenter(const IntervalTree::Index<N>& key) const;
      Point<T, N> scalePoint(const std::array<Rational,N>& point) const;
      const IntervalTree::IndexTree<N>& getTree() const { return *m_tree; }

   private:
      std::unique_ptr< IntervalTree::IndexTree<N>> m_tree;
      BoundingBox<T, N> m_bb;
   };

   template<typename T, size_t N>
   IndexTreeScaled<T, N>::IndexTreeScaled(std::unique_ptr< IntervalTree::IndexTree<N>> tree, BoundingBox<T, N> bb) :
      m_tree(std::move(tree)), m_bb(std::move(bb))
   {}

   template<typename T, size_t N>
   Point<T, N>  IndexTreeScaled<T, N>::scalePoint(const std::array<Rational, N>& point) const
   {
      Point<T, N> result;
      std::transform(point.begin(), point.end(), m_bb.getIntervals().begin(), result.begin(),
         [](const Rational& c, const Interval<T>& intv) {return intv.interpolate(static_cast<T>(c.numerator()) / c.denominator()); });
      return result;
   }

   template<typename T, size_t N>
   Point<T, N> IndexTreeScaled<T, N>::scaleCenter(const IntervalTree::Index<N>& index) const
   {
      const auto center = index.getCenter();
      return scalePoint(center);
   }

} // namespace MeshGeneration
