#include "IndexTreeToSimplices3.h"
//#include "IntervalTreeAdjacentDirection.h"
#include "IntervalTreeIndex.h"
#include "ReferenceShapeCube.h"
#include "TopologyDefines.h"
#include "UniqueHashedPointCollection.h"

using namespace Topology;

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
      const std::array<RatPoint3, NumNodesOnCube> p{
         RatPoint3{ lwr[0],lwr[1],lwr[2]}, RatPoint3{upr[0],lwr[1],lwr[2]}, RatPoint3{lwr[0],upr[1],lwr[2]}, RatPoint3{upr[0],upr[1],lwr[2]},
         RatPoint3{ lwr[0],lwr[1],upr[2]}, RatPoint3{upr[0],lwr[1],upr[2]}, RatPoint3{lwr[0],upr[1],upr[2]}, RatPoint3{upr[0],upr[1],upr[2]} };
      if (str::none_of(moreRefined, std::identity()))
      {
         for (const auto& t : ReferenceShapeCube::getInstance().getStandardSplit())
         {
            Tetrahedrons.emplace_back(std::array<RatPoint3, Topology::NumNodesOnTetrehadron >{ p[t[0]], p[t[1]], p[t[2]], p[t[3]] });
         }
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


std::unique_ptr<Vtk::VtkData> IndexTreeToSimplices3::ToVtkData(const Tetrahedrons& cells, const Vtk::Name& name)
{
   std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(GeomDim3, 0, name);
   UniqueHashedPointCollection<Rational, GeomDim3>  toNodeIndex;
   for (const auto& cell : cells)
   {
      std::array<PointIndex, NumNodesOnTetrehadron> cellNodes;
      for (size_t vertex = 0; const auto & v : cell)
      {
         cellNodes.at(vertex) = toNodeIndex.addIfNew(v);
         ++vertex;
      }
      result->addCell(Vtk::CellType::VTK_TETRA, cellNodes, toNodeIndex, {});
   }
   return result;
}