#include "IndexTreeToSimplices3.h"
//#include "IntervalTreeAdjacentDirection.h"
#include "IntervalTreeIndex.h"
//#include "UniqueHashedPointCollection.h"

namespace
{

   struct ActionSplit
   {
      void operator()(const IntervalTree::Index<GeomDim3>& index);

      const IntervalTree::IndexTree<GeomDim3>& Tree;
      IndexTreeToSimplices3::Tetrahedrons Tetrahedrons;
   };
#if false
   std::pair<RatPoint2, RatPoint2> GetOrientedEdge(const IntervalTree::AdjacentDirection& dir, const std::array<RatPoint2, 4> corners)
   {
      if (dir == IntervalTree::AdjacentDirection{ 0, true })
      {
         return std::make_pair(corners.at(1), corners.at(2));
      }
      if (dir == IntervalTree::AdjacentDirection{ 0, false })
      {
         return std::make_pair(corners.at(3), corners.at(0));
      }
      if (dir == IntervalTree::AdjacentDirection{ 1, true })
      {
         return std::make_pair(corners.at(2), corners.at(3));
      }
      if (dir == IntervalTree::AdjacentDirection{ 1, false })
      {
         return std::make_pair(corners.at(0), corners.at(1));
      }
      throw MyException("GetOrientedEdge should not come here");
   }
#endif
   void ActionSplit::operator()(const IntervalTree::Index<GeomDim3>& index)
   {
      const BoundingBox<Rational, GeomDim3> bb = index.getBbOfCell();
      const auto& neighbors = IntervalTree::GetAdjacentNeighbors3();
      std::array<bool, 2 * GeomDim3> moreRefined{ false, false, false, false, false, false };
      size_t pos = 0;
      for (const auto& dir : neighbors)
      {
         const auto ngb = index.getAdjacentInDir(dir);
         if (ngb)
         {
            const auto found = Tree.get(*ngb);
            if (found)
            {
               moreRefined[pos] = !Tree.isLeaf(*found.value());
            }
         }
         ++pos;
      }


      const auto lwr = bb.getLower();
      const auto upr = bb.getUpper();
      const RatPoint2 ll(lwr[0], lwr[1]);
      const RatPoint2 ul(upr[0], lwr[1]);
      const RatPoint2 uu(upr[0], upr[1]);
      const RatPoint2 lu(lwr[0], upr[1]);
      throw MyException("not yet implemented");
      if (str::none_of(moreRefined, std::identity()))
      {
         //Tetrahedrons.emplace_back(std::array<RatPoint3, Topology::NumNodesOnTetrehadron >{ ll, ul, uu });
         //Tetrahedrons.emplace_back(std::array<RatPoint3, Topology::NumNodesOnTetrehadron >{ ll, uu, lu });
      }
      else
      {
         throw MyException("not yet implemented");
#if false
         const std::array<RatPoint2, 4> corners{ ll, ul, uu, lu };
         const RatPoint2 center = index.getCenter();
         pos = 0;
         for (const auto& dir : neighbors)
         {
            const auto edge = GetOrientedEdge(dir, corners);
            if (!moreRefined.at(pos))
            {
               Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{center, edge.first, edge.second });
            }
            else
            {
               auto midEdge = (edge.first + edge.second) * Rational{ 1,2 };
               Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{center, edge.first, midEdge });
               Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{center, midEdge, edge.second });
            }
            ++pos;
         }
#endif
      }

   }


}

IndexTreeToSimplices3::Tetrahedrons IndexTreeToSimplices3::Create(const IntervalTree::IndexTree<GeomDim3>& tree)
{
   ActionSplit action{ tree };
   tree.foreachLeaf(action);
   return action.Tetrahedrons;
}
