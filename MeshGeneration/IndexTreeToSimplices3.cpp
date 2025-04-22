
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

   struct ActionRefinedEdges
   {
      void operator()(const IntervalTree::Index<GeomDim3>& index);

      const IntervalTree::IndexTree<GeomDim3>* Tree;
      std::set<IndexTreeToSimplices3::TreeEdge> RefinedTreeEdges;
   };


   void ActionRefinedEdges::operator()(const IntervalTree::Index<GeomDim3>& index)
   {
      if (!Tree->isLeaf(index))
      {
         for (const auto& edge : IndexTreeToSimplices3::getCubeEdgesFromIndex(index))
         {
            RefinedTreeEdges.insert(edge);
         }
      }
   }


   struct ActionSplit
   {
      void operator()(const IntervalTree::Index<GeomDim3>& index);

      const IntervalTree::IndexTree<GeomDim3>& Tree;
      const std::set<IndexTreeToSimplices3::TreeEdge>& RefinedEdges;
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
      const std::array<RatPoint3, Topology::NumNodesOnCube>& bbNodes = IndexTreeToSimplices3::getCubeFromIndex(index);
      std::array<IndexTreeToSimplices3::TreeEdge, Topology::NumEdgesOnCube> cellEdges = IndexTreeToSimplices3::getCubeEdgesFromIndex(index);
      std::array<bool, Topology::NumEdgesOnCube> edgeIsRefined;
      str::transform(cellEdges, edgeIsRefined.begin(), [this](const IndexTreeToSimplices3::TreeEdge& edge) {return RefinedEdges.contains(edge); });

      if (str::none_of(edgeIsRefined, std::identity()))
      {
         for (const auto& t : ReferenceShapeCube::getInstance().getStandardSplit())
         {
            Tetrahedrons.emplace_back(std::array<RatPoint3, Topology::NumNodesOnTetrahedron >{ bbNodes[t[0]], bbNodes[t[1]], bbNodes[t[2]], bbNodes[t[3]] });
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

} // namespace

IndexTreeToSimplices3::Tetrahedrons IndexTreeToSimplices3::Create(const IntervalTree::IndexTree<GeomDim3>& tree)
{
   ActionSplit actionSplit{ tree, getRefinedEdges(tree) };
   tree.foreachLeaf(actionSplit);
   return actionSplit.Tetrahedrons;
}

void IndexTreeToSimplices3::cellsToVtkData(MeshGeneration::ProjectToVtk& vtk, const Tetrahedrons& cells)
{
   const PointClose<double, GeomDim3> predicate;
   const auto [tnodes, points] = toPointCollection(cells, predicate);

   vtk.addTetrahedrons(tnodes, points, std::string(Vtk::itemIndexTree3) + "_mesh");
   vtk.addEdges(tnodes.getAllSortedEdges(), points, std::string(Vtk::itemIndexTree3) + "_edges");
}

std::array<RatPoint3, Topology::NumNodesOnCube> IndexTreeToSimplices3::getCubeFromIndex(const IntervalTree::Index<GeomDim3>& index)
{
   const BoundingBox<Rational, GeomDim3> bb = index.getBbOfCell();
   std::array<RatPoint3, NumNodesOnCube> result;
   result[0] = RatPoint3{ bb.getInterval(0).getLower(), bb.getInterval(1).getLower() , bb.getInterval(2).getLower() };
   result[1] = RatPoint3{ bb.getInterval(0).getUpper(), bb.getInterval(1).getLower() , bb.getInterval(2).getLower() };
   result[2] = RatPoint3{ bb.getInterval(0).getLower(), bb.getInterval(1).getUpper() , bb.getInterval(2).getLower() };
   result[3] = RatPoint3{ bb.getInterval(0).getUpper(), bb.getInterval(1).getUpper() , bb.getInterval(2).getLower() };


   result[4] = RatPoint3{ bb.getInterval(0).getLower(), bb.getInterval(1).getLower() , bb.getInterval(2).getUpper() };
   result[5] = RatPoint3{ bb.getInterval(0).getUpper(), bb.getInterval(1).getLower() , bb.getInterval(2).getUpper() };
   result[6] = RatPoint3{ bb.getInterval(0).getLower(), bb.getInterval(1).getUpper() , bb.getInterval(2).getUpper() };
   result[7] = RatPoint3{ bb.getInterval(0).getUpper(), bb.getInterval(1).getUpper() , bb.getInterval(2).getUpper() };
   return result;
}

std::set<IndexTreeToSimplices3::TreeEdge> IndexTreeToSimplices3::getRefinedEdges(const IntervalTree::IndexTree<GeomDim3>& tree)
{
   ActionRefinedEdges action{ &tree };
   tree.foreachNode(action);
   return action.RefinedTreeEdges;
}

std::array<IndexTreeToSimplices3::TreeEdge, Topology::NumEdgesOnCube> IndexTreeToSimplices3::getCubeEdgesFromIndex(const IntervalTree::Index<GeomDim3>& index)
{
   std::array<TreeEdge, Topology::NumEdgesOnCube> result;
   const auto cube = IndexTreeToSimplices3::getCubeFromIndex(index);
   const auto* adjacencyC2E = *ReferenceShapeCube::getInstance().getAdjacencies().getAdjacency(Topology::Corner, Topology::Edge);
   for (int edge = 0; edge < Topology::NumEdgesOnCube; ++edge)
   {
      const auto& edgePoints = adjacencyC2E->getConnectedLowers(edge);
      const auto p0 = cube[edgePoints[0]];
      const auto p1 = cube[edgePoints[1]];
      // Edge is sorted
      result[edge] = { std::min(p0, p1), std::max(p0,p1) };
   }
   return result;
}
