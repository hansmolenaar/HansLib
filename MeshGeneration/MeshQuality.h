#pragma once
#include "IPointCollection.h"
#include "MeshGenerationDefines.h"

namespace MeshGeneration
{
   class TrianglesNodes;

   namespace MeshQuality
   {
      double getQuality2WholeMesh(const TrianglesNodes& triangles, const IPointCollection<GeomType, GeomDim2>& points, CellQualityFun<GeomDim2>* qualityFunction);
      double getQuality2(const TrianglesNodes& triangles, std::span<const CellIndex> activeTriangles, const IPointCollection<GeomType, GeomDim2>& points, CellQualityFun<GeomDim2>* qualityFunction);
   }
}