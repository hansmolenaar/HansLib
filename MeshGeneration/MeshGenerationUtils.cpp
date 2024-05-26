#include "MeshGenerationUtils.h"
#include "IntervalTreeAction.h"

#include <sstream>

using namespace MeshGeneration;

void MeshGeneration::LogSortedEdgeNodes(Logger& logger, const  SortedEdgeNodes& sortedEdgeNodes, std::string header)
{
   logger.logLine(header + std::to_string(sortedEdgeNodes.front()) + "-> " + std::to_string(sortedEdgeNodes.back()));
}

void MeshGeneration::Log(Logger& logger, const std::vector<SortedEdgeNodes>& sortedEdgeNodes, std::string header)
{
   std::vector<std::string> lines;
   lines.push_back(header);
   for (const auto& e : sortedEdgeNodes)
   {
      lines.push_back(std::to_string(e.front()) + " -> " + std::to_string(e.back()));
   }
   logger.logLines(lines);
}

void MeshGeneration::Log(Logger& logger, std::span<const size_t> indices, std::string header)
{
   std::vector<std::string> lines;
   lines.push_back(header);
   for (const auto& i : indices)
   {
      lines.push_back(std::to_string(i));
   }
   logger.logLines(lines);
}

void MeshGeneration::LogTriangles(Logger& logger, const TrianglesNodes& trianglesNodes, std::string header)
{
   std::vector<std::string> lines;
   lines.emplace_back(header);
   for (auto cellId : trianglesNodes.getAllTriangles())
   {
      const auto triangle = trianglesNodes.getTriangleNodes(cellId);
      lines.emplace_back(std::to_string(cellId) + " -> " + std::to_string(triangle[0]) + "  " + std::to_string(triangle[1]) + "  " + std::to_string(triangle[2]));
   }
   logger.logLines(lines);
}

void MeshGeneration::Log(Logger& logger, const IndexTreeToSimplices2::Triangles& triangles, std::string header)
{
   const std::string sep = "  ";
   std::vector<std::string> lines;
   lines.emplace_back(header);
   
   for (const auto& t : triangles)
   {
      std::ostringstream buffer;
      buffer << t.at(0) << sep << t.at(1) << sep << t.at(2);
      lines.push_back(buffer.str());
   }
   logger.logLines(lines);
}

void MeshGeneration::LogBb2(Logger& logger, const BoundingBox<GeomType, GeomDim2>& bb, std::string header)
{
   std::vector<std::string> lines;
   lines.emplace_back(header);
   bool first = true;
   for (const auto& i : bb.getIntervals())
   {
      std::ostringstream buffer;
      buffer << "  ( " << i.getLower() << " , " << i.getUpper() << " )";
      lines.push_back(buffer.str());
   }
   logger.logLines(lines);
}

void MeshGeneration::Log(Logger& logger, IntervalTree::IndexTree<GeomDim2> tree, std::string header)
{
   std::vector<std::string> lines;
   lines.emplace_back(header); 
   IntervalTree::ActionLogLeaves<2> actionLog{ logger };
   tree.foreachLeaf(actionLog);
}

void MeshGeneration::Log(Logger& logger, const std::vector<const IntervalTree::Index<GeomDim2>*>& cells, std::string header)
{
   std::vector<std::string> lines;
   lines.emplace_back(header);
   for (const auto& indx : cells)
   {
      lines.push_back(indx->toString());
   }
   logger.logLines(lines);
}