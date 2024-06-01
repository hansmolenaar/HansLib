#pragma once

#include "Logger.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"
#include "IndexTreeToSimplices2.h"
#include "BoundingBox.h"
#include "IntervalTree.h"
#include "TopologyDefines.h"

namespace MeshGeneration
{
   void LogSortedEdgeNodes(Logger& logger, const  SortedEdgeNodes& sortedEdgeNodes, std::string header = "");
   void Log(Logger& logger, const std::vector<SortedEdgeNodes>& sortedEdgeNodes, std::string header);
   void Log(Logger& logger, std::span<const size_t> indices, std::string header);
   void LogTriangles(Logger& logger, const TrianglesNodes& trianglesNodes, std::string header = "");
   void Log(Logger& logger, const IndexTreeToSimplices2::Triangles& triangles, std::string header);
   void LogBb2(Logger& logger, const BoundingBox<GeomType, GeomDim2>& bb, std::string header);

   void Log(Logger& logger, IntervalTree::IndexTree<GeomDim2> tree, std::string header);
   void Log(Logger& logger, const std::vector<const IntervalTree::Index<GeomDim2>*>& cells, std::string header);

   std::array<Point<GeomType, GeomDim2>, Topology::NumNodesOnTriangle> GetTriangleGeometry(
      const MeshGeneration::TriangleNodes& triangleNodes, 
      const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points);
}