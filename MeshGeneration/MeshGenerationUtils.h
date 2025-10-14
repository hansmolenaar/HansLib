#pragma once

#include "BoundingBox.h"
#include "IndexTreeToSimplices2.h"
#include "IntervalTree.h"
#include "Logger.h"
#include "MeshGenerationDefines.h"
#include "TopologyDefines.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   void LogSortedEdgeNodes(Logger& logger, const  Topology::EdgeNodesSorted& sortedEdgeNodes, std::string header = "");
   void Log(Logger& logger, const std::vector<Topology::EdgeNodesSorted>& sortedEdgeNodes, std::string header);
   void Log(Logger& logger, std::span<const size_t> indices, std::string header);
   void LogTriangles(Logger& logger, const TrianglesNodes& trianglesNodes, std::string header = "");
   void Log(Logger& logger, const IndexTreeToSimplices2::Triangles& triangles, std::string header);
   void LogBb2(Logger& logger, const BoundingBox<GeomType, GeomDim2>& bb, std::string header);

   void Log(Logger& logger, IntervalTree::IndexTree<GeomDim2> tree, std::string header);
   void Log(Logger& logger, const std::vector<const IntervalTree::Index<GeomDim2>*>& cells, std::string header);

   std::array<Point<GeomType, GeomDim2>, Topology::NumCornersOnTriangle> GetTriangleGeometry(
      const Topology::TriangleNodes& triangleNodes,
      const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points);
}