#pragma once

#include "MeshGenerationDefines.h"
#include "IPointCollection.h"

namespace TriangleNodes2
{
   bool TriangleContainsPoint(const Point2& point,  const MeshGeneration::TriangleNodes& triangleNodes, const IPointCollection<MeshGeneration::GeomType, GeomDim2>& points);
}