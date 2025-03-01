#include "IndexTreeToSimplices2.h"
#include "IntervalTreeAdjacentDirection.h"
#include "IntervalTreeIndex.h"
#include "UniqueHashedPointCollection.h"

namespace
{

   struct ActionSplit
   {
      void operator()(const IntervalTree::Index<2>& index);

      const IntervalTree::IndexTree<2>& Tree;
      IndexTreeToSimplices2::Triangles Triangles;
   };

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

   void ActionSplit::operator()(const IntervalTree::Index<2>& index)
   {
      const BoundingBox<Rational, 2> bb = index.getBbOfCell();
      const auto& neighbors = IntervalTree::GetAdjacentNeighbors2();
      std::array<bool, 4> moreRefined{ false, false, false, false };
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

      if (str::none_of(moreRefined, [](bool b) {return b; }))
      {
         Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{ ll, ul, uu });
         Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{ ll, uu, lu });
      }
      else
      {
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
      }
   }
}

IndexTreeToSimplices2::Triangles IndexTreeToSimplices2::Create(const IntervalTree::IndexTree<2>& tree)
{
   ActionSplit action{ tree };
   tree.foreachLeaf(action);
   return action.Triangles;
}

std::unique_ptr<Vtk::VtkData> IndexTreeToSimplices2::ToVtkData(const Triangles& cells, const Vtk::Name& name)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(GeometryDimension, 0, name);
   UniqueHashedPointCollection<Rational, GeometryDimension>  toNodeIndex;
   for (const auto& cell : cells)
   {
      std::array<PointIndex, ReferenceShapePolygon::TriangleNumCorners> cellNodes;
      for (size_t vertex = 0; const auto & v : cell)
      {
         cellNodes.at(vertex) = toNodeIndex.addIfNew(v);
         ++vertex;
      }
      result->addCell(Vtk::CellType::VTK_TRIANGLE, cellNodes, toNodeIndex, {});
   }
   return result;
}