#pragma once

#include "MeshGenerationDefines.h"

class  CellQuality2MinimumAngle
{
public:
   double operator()(const Geometry::Simplex<MeshGeneration::GeomType, GeomDim2>& triangle) const;
   static MeshGeneration::CellQuality2 GetCellQuality2();
};