#pragma once

#include "IReferenceShape.h"
#include "TopologicalAdjacencies.h"

class ReferenceShapeCube : public IReferenceShape
{
public:

   static const ReferenceShapeCube& getInstance();
   const ITopologicalAdjacencies& getAdjacencies() const override;
private:
   ReferenceShapeCube();
   std::unique_ptr<TopologicalAdjacencies> m_adjacencies;
};