#pragma once

#include "IReferenceShape.h"

class ReferenceShapePolygon : public IReferenceShape
{
public:
   static const IReferenceShape& Get(int numCorners);
   static const IReferenceShape& GetTriangle();

   static constexpr int TriangleNumCorners = 3;
   static constexpr int TriangleNumEdges = 3;
};
