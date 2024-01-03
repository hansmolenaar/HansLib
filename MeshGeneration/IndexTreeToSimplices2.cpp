#include "IndexTreeToSimplices2.h"
#include "IntervalTreeIndex.h"

namespace
{

   struct ActionSplit
   {
      void operator()(const IntervalTree::Index<2>& index);
      std::vector<std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>> Triangles;
   };

   void ActionSplit::operator()(const IntervalTree::Index<2>& index)
   {
      const BoundingBox<Rational, 2> bb = index.getBbOfCell();
      const auto lwr = bb.getLower();
      const auto upr = bb.getUpper();
      const RatPoint2 ll(lwr[0], lwr[1]);
      const RatPoint2 ul(upr[0], lwr[1]);
      const RatPoint2 uu(upr[0], upr[1]);
      const RatPoint2 lu(lwr[0], upr[1]);
      Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{ ll, ul, uu });
      Triangles.emplace_back(std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners >{ ll, uu, lu });
   }
}

std::vector<std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>> IndexTreeToSimplices2::Create(const IntervalTree::IndexTree<2>& tree)
{
   ActionSplit action;
   tree.foreachLeaf(action);
   return action.Triangles;
}

std::unique_ptr<Vtk::VtkData> IndexTreeToSimplices2::ToVtkData(const std::vector<std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>>& cells)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(GeometryDimension, 0);
   std::map<RatPoint2, Vtk::NodeIndex> toNodeIndex;
   for (const auto& cell : cells)
   {
      std::array<Vtk::NodeIndex, ReferenceShapePolygon::TriangleNumCorners> cellNodes{ -1, -1, -1 };
      size_t vertex = 0;
      for (const auto& v : cell)
      {
         if (!toNodeIndex.contains(v))
         {
            toNodeIndex[v] = static_cast<Vtk::NodeIndex>(toNodeIndex.size());

            std::array<float, GeometryDimension> coordinates;
            coordinates.at(0) = static_cast<float>(v[0].numerator()) / v[0].denominator();
            coordinates.at(1) = static_cast<float>(v[1].numerator()) / v[1].denominator();
            result->addNode(coordinates);
         }

         cellNodes.at(vertex) = toNodeIndex.at(v);
         ++vertex;
      }
      result->addCell(Vtk::CellType::VTK_TRIANGLE, cellNodes, {});
   }
   return result;
}