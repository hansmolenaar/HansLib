#pragma once

#include "EdgeNodesSorted.h"
#include "IManifoldReconstruction.h"
#include "MeshGenerationDefines.h"

#include <memory>

namespace MeshGeneration
{
   class TrianglesNodes;

   struct EdgeFlipStrategy
   {
      double treshold; // Ignore cells with quality above
      int maxNumSweeps;
   };

   class EdgeFlip
   {
   public:
      EdgeFlip(
         TrianglesNodes& trianglesNodes,
         CellQuality2Fun* getCellQuality,
         const IPointCollection2& pointCollection,
         const std::vector<const IManifoldReconstruction*>& reconstructions);

      int execute(const EdgeFlipStrategy& strategy);
      bool isFlippable(const Topology::EdgeNodesSorted& edge) const;

   private:
      TrianglesNodes& m_trianglesNodes;
      CellQuality2Fun* m_cellQuality;
      const IPointCollection2& m_pointCollection;
      std::vector<const IManifoldReconstruction*> m_reconstructions;
   };

}