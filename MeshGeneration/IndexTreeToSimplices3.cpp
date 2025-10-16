
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
      const std::array<RatPoint3, NumCornersOnCube>& bbNodes = IndexTreeToSimplices3::getCubeFromIndex(index);
      std::array<IndexTreeToSimplices3::TreeEdge, NumEdgesOnCube> cellEdges = IndexTreeToSimplices3::getCubeEdgesFromIndex(index);
      std::array<bool, NumEdgesOnCube> edgeIsRefined;
      str::transform(cellEdges, edgeIsRefined.begin(), [this](const IndexTreeToSimplices3::TreeEdge& edge) {return RefinedEdges.contains(edge); });

      if (str::none_of(edgeIsRefined, std::identity()))
      {
         for (const auto& t : ReferenceShapeCube::getInstance().getStandardSplit())
         {
            Tetrahedrons.emplace_back(std::array<RatPoint3, Topology::NumCornersOnTetrahedron >{ bbNodes[t[0]], bbNodes[t[1]], bbNodes[t[2]], bbNodes[t[3]] });
         }
      }
      else
      {
         auto rp = [](const RatPoint3& p0, const RatPoint3& p1) {return p0 + p1; };
         const RatPoint3 cellCenterPoint = std::accumulate(bbNodes.begin(), bbNodes.end(), RatPoint3{ 0,0,0 }, rp) / NumCornersOnCube;
         const auto* adjacencyE2F = *ReferenceShapeCube::getInstance().getAdjacencies().getAdjacency(Topology::Edge, Topology::Face);
         const auto* adjacencyC2F = *ReferenceShapeCube::getInstance().getAdjacencies().getAdjacency(Topology::Corner, Topology::Face);
         for (auto face = 0; face < NumFacesOnCube; ++face)
         {
            const auto& edgesOnFace = adjacencyE2F->getConnectedLowers(face);
            const auto& cornersOnFace = adjacencyC2F->getConnectedLowers(face);
            auto addFacePoints = [&bbNodes](const RatPoint3& rp, int crnr) {return rp + bbNodes[crnr]; };
            const RatPoint3 faceCenterPoint = std::accumulate(cornersOnFace.begin(), cornersOnFace.end(), RatPoint3{ 0,0,0 }, addFacePoints) / NumCornersOnSquare;
            const bool noEdgeSplit = str::none_of(edgesOnFace, [&edgeIsRefined](int edge) {return edgeIsRefined[edge]; });
            if (noEdgeSplit)
            {
            }
            else
            {

            }
         }
         //throw MyException("not yet implemented");
#if false


         foreach(var faceLocal in cubeShape.GetFaces())
         {
            var anyEdgeSplit = cubeShape.GetEdgesOfFace(faceLocal).Any(e = > edgeCenterNode[e] != NodeUndefined.Value);
            var allEdgeSplit = cubeShape.GetEdgesOfFace(faceLocal).All(e = > edgeCenterNode[e] != NodeUndefined.Value);
            if (!anyEdgeSplit)
            {
               // Create 2 tets for the normal face split
               var fnodes = cubeShape.GetHyperFaceVertices(faceLocal).ToArray();
               var diag02 = Enumerable.Range(0, cubeShape.TetsInStandardSplitCount).
                  Any(t = > cubeShape.GetTetrahedronInSplit(t).Contains(fnodes[0]) && cubeShape.GetTetrahedronInSplit(t).Contains(fnodes[2]));
               var diag13 = Enumerable.Range(0, cubeShape.TetsInStandardSplitCount).
                  Any(t = > cubeShape.GetTetrahedronInSplit(t).Contains(fnodes[1]) && cubeShape.GetTetrahedronInSplit(t).Contains(fnodes[3]));
               GenericUtils.Assert(diag02 ^ diag13);

               var faceNodes = cubeShape.GetHyperFaceVertices(faceLocal).Select(v = > cubeNodes[v]).ToArray();
               if (diag02)
               {
                  yield return NodesTetrahedron.Create(centerCellNode, faceNodes[2], faceNodes[0], faceNodes[1]);
                  yield return NodesTetrahedron.Create(centerCellNode, faceNodes[0], faceNodes[2], faceNodes[3]);
               }
               else
               {
                  yield return NodesTetrahedron.Create(centerCellNode, faceNodes[1], faceNodes[3], faceNodes[0]);
                  yield return NodesTetrahedron.Create(centerCellNode, faceNodes[3], faceNodes[1], faceNodes[2]);
               }
            }
            else
            {
               var centerFacePoint = PointN.CreateEmpty(3);
               centerFacePoint.Center(cubeShape.GetHyperFaceVertices(faceLocal).Select(v = > cubeNodes[v]).Select(n = > nodeGeometry.GetNodeGeometry(n)));
               var centerFaceNode = nodeGeometry.CreateNodeWithUniqueGeometry(centerFacePoint);

               if (allEdgeSplit)
               {
                  var fcorners = cubeShape.GetHyperFaceVertices(faceLocal).ToArray();
                  var fnodeS = fcorners.Select(c = > cubeNodes[c]).ToArray();
                  var diag02 = Enumerable.Range(0, cubeShape.TetsInStandardSplitCount).
                     Any(t = > cubeShape.GetTetrahedronInSplit(t).Contains(fcorners[0]) && cubeShape.GetTetrahedronInSplit(t).Contains(fcorners[2]));
                  var diag13 = Enumerable.Range(0, cubeShape.TetsInStandardSplitCount).
                     Any(t = > cubeShape.GetTetrahedronInSplit(t).Contains(fcorners[1]) && cubeShape.GetTetrahedronInSplit(t).Contains(fcorners[3]));
                  int diagMod2 = diag02 ? 0 : 1;
                  var f2e = cubeShape.TopologicalAdjacencies.GetAdjacency(TopologyDimension.Face, TopologyDimension.Edge);
                  for (int corner = 0; corner < fcorners.Count(); ++corner)
                  {
                     bool isDiag = ((corner % 2) == diagMod2);
                     // Find edges in order
                     int edgePos = -1;
                     bool ready = false;
                     foreach(var edge0 in f2e.GetConnectedLowers(faceLocal))
                     {
                        ++edgePos;
                        int edge1 = f2e.GetConnectedLowers(faceLocal).ElementAt((edgePos + 1) % 4);
                        int v00, v01, v10, v11;
                        cubeShape.GetOrderedVerticesOfEdgeInFace(faceLocal, edge0, out v00, out v01);
                        cubeShape.GetOrderedVerticesOfEdgeInFace(faceLocal, edge1, out v10, out v11);
                        GenericUtils.Assert(v01 == v10);
                        if (cubeNodes[v01] == fnodeS[corner])
                        {
                           GenericUtils.Assert(!ready);
                           ready = true;
                           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                           int edgeCenterNode0 = edgeCenterNode[edge0];
                           int edgeCenterNode1 = edgeCenterNode[edge1];
                           // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                           if (isDiag)
                           {
                              yield return NodesTetrahedron.Create(centerCellNode, centerFaceNode, edgeCenterNode[edge0], fnodeS[corner]);
                              yield return NodesTetrahedron.Create(centerCellNode, centerFaceNode, fnodeS[corner], edgeCenterNode[edge1]);
                           }
                           else
                           {
                              yield return NodesTetrahedron.Create(centerCellNode, centerFaceNode, edgeCenterNode[edge0], edgeCenterNode[edge1]);
                              yield return NodesTetrahedron.Create(centerCellNode, edgeCenterNode[edge0], fnodeS[corner], edgeCenterNode[edge1]);
                           }
                        }
                     }
                     GenericUtils.Assert(ready);
                  }
               }
               else
               {
                  // At least 1 edge split
                  foreach(var edge in cubeShape.GetEdgesOfFace(faceLocal))
                  {
                     int v1;
                     int v2;
                     cubeShape.GetOrderedVerticesOfEdgeInFace(faceLocal, edge, out v1, out v2);
                     int vert1 = cubeNodes[v1];
                     int vert2 = cubeNodes[v2];
                     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                     int edgeCenterNodE = edgeCenterNode[edge];
                     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                     if (edgeCenterNode[edge] == NodeUndefined.Value)
                     {
                        // edge not split
                        yield return NodesTetrahedron.Create(centerCellNode, centerFaceNode, vert1, vert2);
                     }
                     else
                     {
                        // edge split, create 2 tets
                        yield return NodesTetrahedron.Create(centerCellNode, centerFaceNode, vert1, edgeCenterNode[edge]);
                        yield return NodesTetrahedron.Create(centerCellNode, centerFaceNode, edgeCenterNode[edge], vert2);
                     }
                  }
               }
            }
         }
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
      std::array<PointIndex, NumCornersOnTetrahedron> cellNodes;
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

   vtk.addTetrahedronsAndBoundaries(tnodes, points, std::string("IndexTree3_mesh"));
   vtk.addEdges(tnodes.getAllSortedEdges(), points, std::string("IndexTree3_edges"));
}

std::array<RatPoint3, Topology::NumCornersOnCube> IndexTreeToSimplices3::getCubeFromIndex(const IntervalTree::Index<GeomDim3>& index)
{
   const BoundingBox<Rational, GeomDim3> bb = index.getBbOfCell();
   std::array<RatPoint3, NumCornersOnCube> result;
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
      // Edge is sorted
      result[edge] = TreeEdge(cube[edgePoints[0]], cube[edgePoints[1]]);
   }
   return result;
}
