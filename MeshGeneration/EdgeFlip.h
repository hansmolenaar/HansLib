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
         const IUniquePointCollection2& pointCollection,
         const std::vector<std::unique_ptr<IManifoldReconstruction>>& reconstructions);

      void execute(const EdgeFlipStrategy& strategy);
      bool isFlippable(const Topology::EdgeNodesSorted& edge) const;

   private:
      TrianglesNodes& m_trianglesNodes;
      CellQuality2Fun* m_cellQuality;
      const IUniquePointCollection2& m_pointCollection;
      std::vector<const IManifoldReconstruction*> m_reconstructions;
   };

}