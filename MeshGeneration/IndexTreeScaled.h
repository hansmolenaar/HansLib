#pragma once

#include "Interval.h"
#include "BoundingBox.h"
#include "IntervalTree.h"
#include "IntervalTreeIndex.h"
#include "Defines.h"

namespace MeshGeneration
{
   template<typename T, int N>
   class IndexTreeScaled
   {
   public:
      IndexTreeScaled(std::unique_ptr< IntervalTree::IndexTree<N>> tree, BoundingBox<T, N> bb);
      Point<T, N> scaleCenter(typename const IntervalTree::Index<N>& key) const;
      const IntervalTree::IndexTree<N>& getTree() const { return *m_tree; }

   private:
      std::unique_ptr< IntervalTree::IndexTree<N>> m_tree;
      BoundingBox<T, N> m_bb;
   };

   template<typename T, int N>
   IndexTreeScaled<T, N>::IndexTreeScaled(std::unique_ptr< IntervalTree::IndexTree<N>> tree, BoundingBox<T, N> bb) :
      m_tree(std::move(tree)), m_bb(std::move(bb))
   {}

   template<typename T, int N>
   Point<T, N> IndexTreeScaled<T, N>::scaleCenter(typename const IntervalTree::Index<N>& index) const
   {
      Point<T, N> result;
      const auto center = index.getCenter();
      std::transform(center.begin(), center.end(), m_bb.getIntervals().begin(), result.begin(),
         [](const Rational& c, const Interval<T>& intv) {return intv.interpolate(static_cast<T>(c.numerator()) / c.denominator()); });
      return result;
   }

} // namespace MeshGeneration
