#include "IndexTreeToSimplices3.h"
#include "IntervalTreeIndex.h"
#include "ITopologicalAdjacency.h"
#include "PointClose.h"
#include "ReferenceShapeCube.h"
#include "ReferenceShapeTetrahedron.h"
#include "TetrahedronsNodes.h"
#include "TopologyDefines.h"
#include "UniquePointCollectionBinning.h"

using namespace Topology;
using namespace Vtk;
using namespace MeshGeneration;

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
            Tetrahedrons.emplace_back(std::array<RatPoint3, Topology::NumNodesOnTetrahedron >{ p[t[0]], p[t[1]], p[t[2]], p[t[3]] });
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

   std::pair<TetrahedronsNodes, UniquePointCollectionBinning<GeomDim3>> toPointCollection(const IndexTreeToSimplices3::Tetrahedrons& cells, const IGeometryPredicate<double, 3>& predicate)
   {
      TetrahedronsNodes tnodes;
      UniquePointCollectionBinning<GeomDim3> points(predicate, std::vector<Point3>{{0, 0, 0}, { 1,1,1 }});
      for (const auto& cell : cells)
      {
         std::array<PointIndex, NumNodesOnTetrahedron> cellNodes;
         for (size_t vertex = 0; const auto & v : cell)
         {
            cellNodes.at(vertex) = points.addIfNew(PointUtils::toPoint(v));
            ++vertex;
         }
         tnodes.addTetrahedron(TetrahedronNodesOriented(cellNodes));
      }
      return { tnodes, points };
   }

   std::unique_ptr<VtkData> toVtkDataCells(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const Name& name)
   {
      std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(GeomDim3, 0, name);
      for (const auto cellId : tnodes.getAllTetrahedrons())
      {
         const auto& cellNodes = tnodes.getTetrahedronNodes(cellId);
         result->addCell(Vtk::CellType::VTK_TETRA, cellNodes, points, {});
      }
      return result;
   }

   std::unique_ptr<VtkData> toVtkDataEdges(const TetrahedronsNodes& tnodes, const IPointCollection3& points, const Name& name)
   {
      std::unique_ptr< Vtk::VtkData> result = std::make_unique< Vtk::VtkData>(GeomDim3, 0, name);
      for (const auto& edge : tnodes.getAllSortedEdges())
      {
         std::array<PointIndex, NumNodesOnEdge> edgeNodes{ edge[0], edge[1] };
         result->addCell(Vtk::CellType::VTK_LINE, edgeNodes, points, {});
      }
      return result;
   }

} // namespace

IndexTreeToSimplices3::Tetrahedrons IndexTreeToSimplices3::Create(const IntervalTree::IndexTree<GeomDim3>& tree)
{
   ActionSplit action{ tree };
   tree.foreachLeaf(action);
   return action.Tetrahedrons;
}


std::vector< std::unique_ptr<Vtk::VtkData>> IndexTreeToSimplices3::cellsToVtkData(const Tetrahedrons& cells, const std::string& projectName)
{
   const PointClose<double, GeomDim3> predicate;
   const auto [tnodes, points] = toPointCollection(cells, predicate);

   std::vector< std::unique_ptr<Vtk::VtkData>> result;
   result.emplace_back(toVtkDataCells(tnodes, points, { projectName, std::string(Vtk::itemIndexTree3) }));
   result.emplace_back(toVtkDataEdges(tnodes, points, { projectName, std::string(Vtk::itemIndexTree3_edges) }));

   return result;
}
