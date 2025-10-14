#pragma once

#include "IPointCollection.h"
#include "MeshGenerationDefines.h"
#include "TrianglesNodes.h"

namespace MeshGeneration
{
   struct MeshStatistics
   {
      size_t NumberOfNodes;
      size_t NumberOfCells;
      double Quality;

      bool operator==(const MeshStatistics& other) const;
      static MeshStatistics Create2(const TrianglesNodes& triangles, const IPointCollection<GeomType, GeomDim2>& points, CellQualityFun<GeomDim2>* qualityFunction);
   };

}